#include "bltpch.h"
#include "NetworkServer.h"

namespace DND
{

	NetworkServer::NetworkServer(SocketAddress address)
		: m_OnReceivedPacket(), m_BoundAddress(std::move(address)), m_Socket(AddressFamily::INET)
	{
		m_OnReceivedPacket.Subscribe([this](ReceivedPacket& packet)
		{
			std::vector<ListenerInfo>& callbacks = m_Callbacks[packet.Type];
			for (int i = callbacks.size() - 1; i >= 0; i--)
			{
				ListenerInfo& callbackInfo = callbacks[i];
				if (callbackInfo.Callback(packet))
				{
					callbacks.erase(callbacks.begin() + i);
				}
				packet.Packet.Reset(PACKET_HEADER_SIZE);
			}
			ListenerResponse response;
			return response;
		});
	}

	const SocketAddress& NetworkServer::Address() const
	{
		return m_BoundAddress;
	}

	void NetworkServer::Initialize()
	{
		while (!Bind())
		{
			sockaddr_in& currentAddress = *(sockaddr_in*)&m_BoundAddress.m_SockAddr;
			uint addr = ntohl(currentAddress.sin_addr.S_un.S_addr);
			SetAddress(SocketAddress(addr, ntohs(currentAddress.sin_port) + 1));
		}
		BLT_INFO("STARTED SERVER ON {}", m_BoundAddress);
		std::thread listenerThread([this]()
		{
			constexpr uint MAX_PACKET_SIZE = 1024;
			byte PACKET_BUFFER[MAX_PACKET_SIZE];
			while (true)
			{
				SocketAddress fromAddress;
				int bytesReceived = m_Socket.RecvFrom(PACKET_BUFFER, MAX_PACKET_SIZE, &fromAddress);
				if (bytesReceived >= PACKET_HEADER_SIZE)
				{
					InputMemoryStream packet(bytesReceived);
					memcpy(packet.GetBufferPtr(), PACKET_BUFFER, bytesReceived);
					id_t packetValidator;
					PacketType packetType;
					Deserialize(packet, packetValidator);
					Deserialize(packet, packetType);
					if (packetValidator == PACKET_VALIDATOR)
					{
						if (packetType == PacketType::TerminateLocalServer)
						{
							break;
						}
						ReceivedPacket packetEventArgs;
						packetEventArgs.Type = packetType;
						packetEventArgs.FromAddress = fromAddress;
						packetEventArgs.Packet = std::move(packet);
						m_OnReceivedPacket.Post(std::move(packetEventArgs));
					}
					else
					{
						BLT_ERROR("INVALID PACKET VALIDATOR RECEIVED {}", packetValidator);
					}
				}
				else
				{
					BLT_ERROR("INVALID PACKET RECEIVED, SIZE={}", bytesReceived);
				}
			}
			BLT_INFO("STOPPED SERVER ON {}", m_BoundAddress);
			EventManager::Post<ServerShutdownEvent>();
		});
		listenerThread.detach();
	}

	void NetworkServer::Terminate(NetworkServer::ServerTerminatedCallback callback)
	{
		EventManager::Subscribe<ServerShutdownEvent>([callback = std::move(callback)](ServerShutdownEvent& e)
		{
			callback();
			ListenerResponse response;
			response.UnsubscribeListener = true;
			return response;
		});
		TerminateServer();
	}

	void NetworkServer::AddPacketListener(PacketType type, NetworkServer::PacketReceivedCallback listener)
	{
		AddPacketListenerTimeout(type, -1.0f, std::move(listener), []()
		{
		
		});
	}

	void NetworkServer::AddPacketListenerTimeout(PacketType type, float timeoutSeconds, NetworkServer::PacketReceivedCallback listener, NetworkServer::ListenerTimeoutCallback timeoutCallback)
	{
		m_Callbacks[type].push_back({ std::move(listener), timeoutSeconds, std::move(timeoutCallback) });
	}

	void NetworkServer::ClearPacketListeners()
	{
		m_Callbacks.clear();
	}

	void NetworkServer::Update(float deltaTime)
	{
		for (auto& pair : m_Callbacks)
		{
			for (int i = pair.second.size() - 1; i >= 0; i--)
			{
				ListenerInfo& info = pair.second[i];
				if (info.SecondsRemaining > 0.0f)
				{
					info.SecondsRemaining -= deltaTime;
					if (info.SecondsRemaining <= 0.0f)
					{
						info.TimeoutCallback();
					}
				}
			}
		}
	}

	bool NetworkServer::Bind()
	{
		int result = m_Socket.Bind(m_BoundAddress);
		return result == NO_ERROR;
	}

	void NetworkServer::SetAddress(SocketAddress address)
	{
		m_BoundAddress = std::move(address);
	}

	void NetworkServer::TerminateServer()
	{
		SendPacket(Address(), TerminateServerPacket());
	}

}