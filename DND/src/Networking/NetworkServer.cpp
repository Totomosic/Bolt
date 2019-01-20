#include "bltpch.h"
#include "NetworkServer.h"

namespace DND
{

	PacketValidator::PacketValidator()
		: m_CurrentPacketId(0)
	{
	
	}

	id_t PacketValidator::GetNextPacketId()
	{
		return m_CurrentPacketId++;
	}

	bool PacketValidator::ValidateIncomingPacketId(id_t id)
	{
		return true;
	}

	NetworkServer::NetworkServer()
		: OnShutdown(SERVER_SHUTDOWN_EVENT),
		OnHelloPacket(SERVER_RECEIVED_HELLO_PACKET_EVENT), OnWelcomePacket(SERVER_RECEIVED_WELCOME_PACKET_EVENT), OnIntroductionPacket(SERVER_RECEIVED_INTRODUCTION_PACKET_EVENT), OnDisconnectPacket(SERVER_RECEIVED_DISCONNECT_PACKET_EVENT),
		OnPlayerMovePacket(SERVER_RECEIVED_PLAYER_MOVE_PACKET_EVENT), OnCastSpellPacket(SERVER_RECEIVED_CAST_SPELL_PACKET_EVENT), OnStatUpdatePacket(SERVER_RECEIVED_STAT_PACKET_EVENT), OnDeathPacket(SERVER_RECEIVED_DEATH_PACKET_EVENT),
		m_IsRunning(false), m_Address(), m_Socket(), m_Validators()
	{
		ResetSocket();
	}

	void NetworkServer::SetAddress(const SocketAddress& address)
	{
		if (m_Address != address)
		{
			m_Address = address;
		}
	}

	void NetworkServer::Bind()
	{
		m_Socket.Bind(m_Address);
	}

	void NetworkServer::Initialize(bool runListenThread)
	{
		m_Validators.clear();		
		m_IsRunning = true;
		if (runListenThread)
		{
			ClearSocket();
			BLT_CORE_WARN("LISTENING ON {}", m_Address.ToString());
			RunListenThread();
		}
	}

	void NetworkServer::Terminate()
	{
		if (m_IsRunning)
		{
			m_IsRunning = false;
			StopListeningThread();
			m_Socket.Shutdown();
		}
		m_IsRunning = false;
		ResetSocket();
	}

	void NetworkServer::RunListenThread()
	{
		std::thread listeningThread([this]()
		{
			constexpr size_t MAX_PACKET_SIZE = 1024;
			while (true)
			{
				byte buffer[MAX_PACKET_SIZE];
				SocketAddress addr;
				int bytesReceived = m_Socket.RecvFrom(buffer, MAX_PACKET_SIZE, &addr);
				if (bytesReceived > 4)
				{
					InputMemoryStream stream(bytesReceived);
					memcpy(stream.GetBufferPtr(), buffer, bytesReceived);
					id_t pId;
					PacketType pType;
					stream.Read(&pId);
					stream.Read((byte*)&pType);

					if (pType == PacketType::LocalSocketTerminate)
					{
						break;
					}

					std::unique_ptr<ReceivedPacketEvent> e = std::make_unique<ReceivedPacketEvent>();
					e->FromAddress = addr;
					e->Server = this;
					e->PacketId = pId;
					e->Type = pType;
					e->Packet = std::move(stream);

					PacketValidator& validator = m_Validators[addr];
					if (validator.ValidateIncomingPacketId(pId))
					{
						DispatchPacketEvent(GetEventDispatcher(pType), std::move(e));
					}

					if (pType == PacketType::Disconnect)
					{
						m_Validators.erase(addr);
					}
				}
				else
				{
					BLT_CORE_ERROR("INVALID PACKET RECEIVED");
					break;
				}
			}

			std::unique_ptr<ServerShutdownEvent> e = std::make_unique<ServerShutdownEvent>();
			OnShutdown.Post(std::move(e));

		});
		listeningThread.detach();
	}

	void NetworkServer::ClearSocket()
	{
		m_Socket.SetBlocking(false);
		byte buffer[1024];
		while (m_Socket.RecvFrom(buffer, 1024, nullptr) != 0)
		{
			BLT_CORE_INFO("CLEARING WAS REQUIRED");
		}
		m_Socket.SetBlocking(true);
	}

	void NetworkServer::ResetSocket()
	{
		m_Socket = UDPsocket();
		m_Address = SocketAddress();
	}

	void NetworkServer::StopListeningThread()
	{
		DisconnectPacket dcPacket;
		for (auto& pair : m_Validators)
		{
			SendPacket(pair.first, dcPacket);
		}

		OutputMemoryStream packet;
		id_t packetId = 0;
		PacketType type = PacketType::LocalSocketTerminate;
		Serialize(packet, packetId);
		Serialize(packet, (byte)type);
		int bytesSent = m_Socket.SendTo(m_Address, packet.GetBufferPtr(), packet.GetRemainingDataSize());
		BLT_ASSERT(bytesSent == packet.GetRemainingDataSize(), "Unable to send");
	}

	void NetworkServer::DispatchPacketEvent(EventDispatcher<ReceivedPacketEvent>& dispatcher, std::unique_ptr<ReceivedPacketEvent>&& args)
	{
		dispatcher.Post(std::move(args));
	}

	EventDispatcher<ReceivedPacketEvent>& NetworkServer::GetEventDispatcher(PacketType type)
	{
		switch (type)
		{
		case PacketType::Hello:
			return OnHelloPacket;
		case PacketType::Welcome:
			return OnWelcomePacket;
		case PacketType::Introduce:
			return OnIntroductionPacket;
		case PacketType::Disconnect:
			return OnDisconnectPacket;
		case PacketType::PlayerMove:
			return OnPlayerMovePacket;
		case PacketType::CastSpell:
			return OnCastSpellPacket;
		case PacketType::StatUpdate:
			return OnStatUpdatePacket;
		case PacketType::Death:
			return OnDeathPacket;
		}
		BLT_ASSERT(false, "Unable to find dispatcher for type");
		return *(EventDispatcher<ReceivedPacketEvent>*)nullptr;
	}

}