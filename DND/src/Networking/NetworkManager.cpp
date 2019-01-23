#include "bltpch.h"
#include "NetworkManager.h"

namespace DND
{

	SocketAddress NetworkManager::EC2_SERVER_ADDRESS;

	NetworkManager::NetworkManager()
		: m_Address(), m_Server(), m_Connections(), m_Objects()
	{
		EC2_SERVER_ADDRESS = SocketAddress("ec2-18-219-148-3.us-east-2.compute.amazonaws.com", 12345);
	}

	const AddressPair& NetworkManager::Address() const
	{
		return m_Address;
	}

	void NetworkManager::SetAddress(const AddressPair& address)
	{
		m_Address = address;
	}

	void NetworkManager::Initialize(const SocketAddress& bindAddress, NetworkManager::OnInitCallback callback)
	{
		m_Connections.ClearConnections();
		m_Address.PrivateEndpoint = bindAddress;
		m_Server.Initialize(bindAddress, [netManager = this, server = &m_Server, callback = std::move(callback)]()
		{
			server->AddPacketListener(PacketType::ConnectToAddress, [netManager = netManager](ReceivedPacketEvent& e)
			{
				ConnectToAddressPacket packet;
				Deserialize(e.Packet, packet);
				netManager->ConnectToWithoutServer(packet.Address, 2000, [](id_t connectionId)
				{
					if (connectionId != GameObject::InvalidID)
					{
						BLT_CORE_WARN("Connected!!!!");
					}
				});
				return true;
			});

			server->AddTemporaryPacketListener(PacketType::AddressPairResponse, [netManager = netManager, callback = std::move(callback)](ReceivedPacketEvent& e)
			{
				AddressPairResponsePacket packet;
				Deserialize(e.Packet, packet);
				netManager->SetAddress(packet.Address);
				callback(netManager->Address());
				return true;
			}, 1);
			RegisterAsHostPacket packet;
			packet.PrivateEndpoint = netManager->Address().PrivateEndpoint;
			server->SendPacket(NetworkManager::EC2_SERVER_ADDRESS, packet);
		});
	}

	void NetworkManager::Exit(NetworkManager::OnExitCallback callback)
	{
		DisconnectHostPacket packet;
		packet.PrivateEndpoint = Address().PrivateEndpoint;
		m_Server.SendPacket(NetworkManager::EC2_SERVER_ADDRESS, packet);
		m_Server.Exit([callback = std::move(callback)]()
		{
			callback();
		});
	}

	void NetworkManager::GetAllHosts(NetworkManager::OnGetHostsCallback callback)
	{
		m_Server.AddTemporaryPacketListener(PacketType::GetAllHostsResponse, [callback = std::move(callback)](ReceivedPacketEvent& e)
		{
			GetAllHostsResponsePacket packet;
			Deserialize(e.Packet, packet);
			callback(std::move(packet.HostAddresses));
			return true;
		}, 1);
		GetAllHostsPacket packet;
		m_Server.SendPacket(NetworkManager::EC2_SERVER_ADDRESS, packet);
	}

	void NetworkManager::ConnectTo(const AddressPair& address, int timeoutMilliseconds, NetworkManager::OnConnectedCallback callback)
	{
		ConnectToWithoutServer(address, timeoutMilliseconds, std::move(callback));

		ConnectToAddressPacket packet;
		packet.Address = address;
		m_Server.SendPacket(NetworkManager::EC2_SERVER_ADDRESS, packet);
	}

	void NetworkManager::ConnectToWithoutServer(const AddressPair& address, int timeoutMilliseconds, NetworkManager::OnConnectedCallback callback)
	{
		Timer* holePunchFunc = &Time::RenderingTimeline().AddTimer(0.5, [this, address]()
		{
			HolepunchPacket packet;
			BLT_CORE_INFO("SENDING HOLEPUNCH PACKET TO {}", address.PrivateEndpoint.ToString());
			m_Server.SendPacket(address.PrivateEndpoint, packet);
			BLT_CORE_INFO("SENDING HOLEPUNCH PACKET TO {}", address.PublicEndpoint.ToString());
			m_Server.SendPacket(address.PublicEndpoint, packet);
		});
		Timer* timeoutFunc = &Time::RenderingTimeline().AddFunction(timeoutMilliseconds / 1000.0f, [this]()
		{
			HolepunchAckPacket packet;
			packet.MyAddress = m_Address;
			m_Server.SendPacket(m_Server.BoundAddress(), packet);
		});
		NetworkServer::ListenerId listenerId = m_Server.AddTemporaryPacketListener(PacketType::Holepunch, [this, address](ReceivedPacketEvent& e)
		{
			HolepunchAckPacket packet;
			packet.MyAddress = m_Address;
			bool result = e.FromAddress == address.PublicEndpoint || e.FromAddress == address.PrivateEndpoint;
			if (result)
			{
				BLT_CORE_INFO("SENDING HOLEPUNCH_ACK PACKET TO {}", e.FromAddress.ToString());
				m_Server.SendPacket(e.FromAddress, packet);
			}
			return result;
		}, 1);
		m_Server.AddTemporaryPacketListener(PacketType::HolepunchAck, [this, address, holePunchFunc, timeoutFunc, listenerId, callback = callback](ReceivedPacketEvent& e)
		{
			HolepunchAckPacket packet;
			Deserialize(e.Packet, packet);
			bool result = address.PublicEndpoint == packet.MyAddress.PublicEndpoint || address.PrivateEndpoint == packet.MyAddress.PrivateEndpoint || e.FromAddress == m_Server.BoundAddress();
			if (result)
			{
				Time::RenderingTimeline().RemoveTimer(holePunchFunc);
				if (e.FromAddress != m_Server.BoundAddress())
				{
					Time::RenderingTimeline().RemoveFunction(timeoutFunc);
					id_t connectionId = m_Connections.AddConnection(address, e.FromAddress);
					BLT_CORE_INFO("ESTABLISHED CONNECTION Id = {0} WITH ADDRESS {1} {2} USING {3}", connectionId, address.PublicEndpoint.ToString(), address.PrivateEndpoint.ToString(), e.FromAddress.ToString());
					ConnectionEstablishedPacket connectionPacket;
					m_Server.SendPacket(e.FromAddress, connectionPacket);
				}
				else
				{
					BLT_CORE_ERROR("CONNECTION REQUEST TIMED OUT");
					m_Server.RemovePacketListener(listenerId);
					callback(GameObject::InvalidID);
				}
			}
			return result;
		}, 1);
		m_Server.AddTemporaryPacketListener(PacketType::ConnectionEstablished, [this, address, callback = callback](ReceivedPacketEvent& e)
		{
			bool result = address.PublicEndpoint == e.FromAddress || address.PrivateEndpoint == e.FromAddress;
			if (result)
			{
				id_t connectionId = m_Connections.GetConnectionId(address);
				BLT_CORE_INFO("CONNECTION {}", connectionId);
				callback(connectionId);
			}
			return result;
		}, 1);
		
	}

	void NetworkManager::DisconnectFrom(id_t connectionId)
	{
		
	}

}