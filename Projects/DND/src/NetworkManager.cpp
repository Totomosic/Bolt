#include "bltpch.h"
#include "NetworkManager.h"

namespace DND
{

	NetworkManager* NetworkManager::s_Instance = nullptr;
	SocketAddress NetworkManager::s_Ec2Address = SocketAddress();

	NetworkManager& NetworkManager::Get()
	{
		if (s_Instance == nullptr)
		{
			s_Instance = new NetworkManager;
		}
		return *s_Instance;
	}

	NetworkManager::NetworkManager()
		: m_Server(SocketAddress()), m_InitializedMutex(), m_Address(), m_IsInitialized(false)
	{
		s_Ec2Address = SocketAddress("ec2-18-218-83-100.us-east-2.compute.amazonaws.com", 12345);

		uint16_t port = 10000;
		std::vector<uint> addresses = SocketUtil::GetIP4Addresses();
		BLT_ASSERT(addresses.size() > 0, "UNABLE TO FIND AVAILABLE ADDRESS");
		SocketAddress addr(addresses[0], port);
		m_Server.SetAddress(addr);
	}

	NetworkServer& NetworkManager::Server()
	{
		return m_Server;
	}

	const AddressPair& NetworkManager::Address() const
	{
		std::scoped_lock<std::mutex> lock(m_InitializedMutex);
		return m_Address;
	}

	bool NetworkManager::IsInitialized() const
	{
		std::scoped_lock<std::mutex> lock(m_InitializedMutex);
		return m_IsInitialized;
	}

	const SocketAddress& NetworkManager::EC2Address() const
	{
		return s_Ec2Address;
	}

	void NetworkManager::Initialize(NetworkManager::InitCallback callback)
	{
		m_Server.Initialize();
		Reconnect(std::move(callback));
	}

	void NetworkManager::Terminate(NetworkManager::TerminateCallback callback)
	{
		std::scoped_lock<std::mutex> lock(m_InitializedMutex);
		m_IsInitialized = false;
		m_Server.Terminate(std::move(callback));
	}

	void NetworkManager::Reconnect(NetworkManager::InitCallback callback)
	{
		m_Server.AddPacketListenerTimeout(PacketType::AddressPairResponse, 5, [this, callback](ReceivedPacket& packet)
		{
			{
				std::scoped_lock<std::mutex> lock(m_InitializedMutex);
				AddressPairResponsePacket response;
				Deserialize(packet.Packet, response);
				m_Address = response.Address;
				m_IsInitialized = true;
				BLT_INFO("NetworkManager Initialized with Public={0}, Private={1}", m_Address.PublicEndpoint, m_Address.PrivateEndpoint);
			}
			callback(true);
			return true;
		},
			[callback]()
		{
			BLT_ERROR("NetworkManager initialization timed out");
			callback(false);
		});

		UploadAddressPacket addressPacket;
		addressPacket.PrivateEndpoint = m_Server.Address();
		m_Server.SendPacket(s_Ec2Address, addressPacket);
	}

	void NetworkManager::RegisterAsHost()
	{
		BLT_INFO("REGISTERING AS HOST Public={0}, Private={1}", Address().PublicEndpoint, Address().PrivateEndpoint);
		RegisterAsHostPacket packet;
		packet.PrivateEndpoint = m_Server.Address();
		m_Server.SendPacket(s_Ec2Address, packet);
	}

	void NetworkManager::RemoveAsHost()
	{
		BLT_INFO("REMOVING AS HOST Public={0}, Private={1}", Address().PublicEndpoint, Address().PrivateEndpoint);
		RemoveAsHostPacket packet;
		packet.PrivateEndpoint = m_Server.Address();
		m_Server.SendPacket(s_Ec2Address, packet);
	}

	void NetworkManager::GetAllHosts(float timeout, NetworkManager::GetHostsCallback callback, NetworkManager::GetHostsTimeoutCallback timeoutCallback)
	{
		m_Server.AddPacketListenerTimeout(PacketType::GetHostsResponse, timeout, [callback = std::move(callback)](ReceivedPacket& packet)
		{
			GetHostsResponsePacket response;
			Deserialize(packet.Packet, response);
			callback(response.Hosts);
			return true;
		},
		[callback = std::move(timeoutCallback)]()
		{
			callback();
		});

		GetAllHostsPacket p;
		m_Server.SendPacket(s_Ec2Address, p);
	}

	void NetworkManager::Update()
	{
		m_Server.Update(Time::RenderingTimeline().DeltaTime());
	}

}