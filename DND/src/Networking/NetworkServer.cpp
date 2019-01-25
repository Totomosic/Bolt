#include "bltpch.h"
#include "NetworkServer.h"

namespace DND
{

	NetworkServer::NetworkServer()
		: m_OnPacketReceived(SERVER_RECEIVED_PACKET_EVENT), m_PacketListeners(), m_SocketAddress(), m_Socket(), m_IsRunning(false)
	{
		m_OnPacketReceived.Subscribe([this](id_t listenerId, ReceivedPacketEvent& e)
		{
			std::vector<ListenerId> removeListeners;
			for (ListenerInfo& listener : m_PacketListeners[e.Type])
			{
				e.Packet.Reset(PACKET_HEADER_SIZE);
				bool result = (*listener.Listener)(e);
				if (listener.ListenCount > 0 && result)
				{
					listener.ListenCount -= 1;
					if (listener.ListenCount <= 0)
					{
						removeListeners.push_back({ e.Type, listener.Listener.get() });
					}
				}
			}
			for (ListenerId& id : removeListeners)
			{
				RemovePacketListener(id);
			}
			return true;
		});
	}

	const SocketAddress& NetworkServer::BoundAddress() const
	{
		return m_SocketAddress;
	}

	bool NetworkServer::IsRunning() const
	{
		return m_IsRunning;
	}

	void NetworkServer::Initialize(const SocketAddress& bindAddress, NetworkServer::OnInitCallback callback)
	{
		BLT_ASSERT(!IsRunning(), "SERVER IS ALREADY RUNNING");
		m_IsRunning = true;
		m_SocketAddress = bindAddress;
		m_Socket = UDPsocket();
		m_Socket.Bind(m_SocketAddress);
		std::thread listenerThread([this]()
		{
			BLT_CORE_INFO("STARTING SERVER ON {}", m_SocketAddress.ToString());
			constexpr int MAX_PACKET_SIZE = 1024;
			byte buffer[MAX_PACKET_SIZE];
			while (true)
			{				
				SocketAddress fromAddress;
				int bytesReceived = m_Socket.RecvFrom(buffer, MAX_PACKET_SIZE, &fromAddress);
				if (bytesReceived >= (int)PACKET_HEADER_SIZE)
				{
					InputMemoryStream packetData((uint)bytesReceived);
					memcpy(packetData.GetBufferPtr(), buffer, bytesReceived);

					id_t packetValidator;
					PacketType packetType;
					Deserialize(packetData, packetValidator);
					Deserialize(packetData, packetType);

					if (packetValidator != PACKET_VALIDATOR)
					{
						BLT_CORE_ERROR("PACKET DOES NOT MATCH VALIDATOR, {}", packetValidator);
						continue;
					}

					BLT_CORE_INFO("RECEIVED PACKET, Type = {0} From = {1}", (int)packetType, fromAddress.ToString());

					if (packetType == PacketType::LocalServerTerminate)
					{
						break;
					}

					std::unique_ptr<ReceivedPacketEvent> e = std::make_unique<ReceivedPacketEvent>();
					e->FromAddress = fromAddress;
					e->Type = packetType;
					e->Server = this;
					e->Packet = std::move(packetData);
					m_OnPacketReceived.Post(std::move(e));
				}
				else
				{
					BLT_CORE_ERROR("INVALID PACKET RECEIVED, BYTE COUNT {}", bytesReceived);
				}
			}
			BLT_CORE_INFO("STOPPING SERVER ON {}", m_SocketAddress.ToString());
			m_IsRunning = false;
			EventManager::Post(SERVER_SHUTDOWN_EVENT);
		});
		listenerThread.detach();
		callback();
	}

	void NetworkServer::Exit(NetworkServer::OnExitCallback callback)
	{
		EventManager::Subscribe(SERVER_SHUTDOWN_EVENT, [callback = std::move(callback)](id_t listenerId, Event& e)
		{
			callback();
			EventManager::Unsubscribe(listenerId);
			return true;
		});
		if (IsRunning())
		{
			StopListeningThread();
		}	
		else
		{
			EventManager::Post(SERVER_SHUTDOWN_EVENT);
		}
		ClearAllListeners();
		m_IsRunning = false;
	}

	NetworkServer::ListenerId NetworkServer::AddPacketListener(PacketType packetType, NetworkServer::PacketListener listener)
	{
		return AddTemporaryPacketListener(packetType, std::move(listener), 0);
	}

	NetworkServer::ListenerId NetworkServer::AddTemporaryPacketListener(PacketType packetType, NetworkServer::PacketListener listener, int count)
	{
		std::vector<ListenerInfo>& listeners = m_PacketListeners[packetType];
		std::unique_ptr<PacketListener> listenerPtr = std::make_unique<PacketListener>();
		*listenerPtr = std::move(listener);
		ListenerId id = { packetType, listenerPtr.get() };
		listeners.push_back({ std::move(listenerPtr), count });
		return id;
	}

	void NetworkServer::RemovePacketListener(const NetworkServer::ListenerId& id)
	{
		if (m_PacketListeners.find(id.Type) != m_PacketListeners.end())
		{
			std::vector<ListenerInfo>& listeners = m_PacketListeners.at(id.Type);
			for (int i = 0; i < listeners.size(); i++)
			{
				if (listeners[i].Listener.get() == id.Listener)
				{
					listeners.erase(listeners.begin() + i);
					return;
				}
			}
		}
	}

	void NetworkServer::ClearPacketListeners(PacketType packetType)
	{
		m_PacketListeners[packetType].clear();
		m_PacketListeners.erase(packetType);
	}

	void NetworkServer::ClearAllListeners()
	{
		m_PacketListeners.clear();
	}

	void NetworkServer::StopListeningThread()
	{
		LocalServerTerminatePacket packet;
		SendPacket(BoundAddress(), packet);
	}

}