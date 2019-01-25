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
				netManager->ConnectToWithoutServer(packet.Address);
				ConnectionEstablishedPacket connectionPacket;
				netManager->Server().SendPacket(packet.Address.PrivateEndpoint, connectionPacket);
				return true;
			});

			server->AddPacketListener(PacketType::Holepunch, [netManager = netManager](ReceivedPacketEvent& e)
			{
				HolepunchAckPacket packet;
				packet.MyAddress = netManager->Address();
				BLT_CORE_INFO("SENDING HOLEPUNCH_ACK PACKET TO {}", e.FromAddress.ToString());
				netManager->Server().SendPacket(e.FromAddress, packet);
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
		id_t connectionId = ConnectToWithoutServer(address);
		Server().AddTemporaryPacketListener(PacketType::ConnectionEstablished, [address, connectionId, callback = std::move(callback)](ReceivedPacketEvent& e)
		{
			if (e.FromAddress == address.PrivateEndpoint)
			{
				BLT_CORE_INFO("ESTABLISHED CONNECTION WITH {0} Id = {1}", address.PrivateEndpoint, connectionId);
				callback(connectionId);
				return true;
			}
			return false;
		}, 1);		

		ConnectToAddressPacket packet;
		packet.Address = address;
		m_Server.SendPacket(NetworkManager::EC2_SERVER_ADDRESS, packet);
	}

	id_t NetworkManager::ConnectToWithoutServer(const AddressPair& address)
	{
		const SocketAddress& addr = address.PrivateEndpoint;
		id_t connectionId = m_Connections.AddConnection(address, addr);
		return connectionId;
	}

	void NetworkManager::DisconnectFrom(id_t connectionId)
	{
		
	}

}