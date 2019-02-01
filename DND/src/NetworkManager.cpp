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
		s_Ec2Address = SocketAddress("ec2-18-219-148-3.us-east-2.compute.amazonaws.com", 12345);

		uint16_t port = 10000;
		std::vector<uint> addresses = SocketUtil::GetIP4Addresses();
		BLT_ASSERT(addresses.size() > 0, "UNABLE TO FIND AVAILABLE ADDRESS");
		SocketAddress addr(addresses[0], port);
		m_Server.SetAddress(addr);
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

	void NetworkManager::Initialize(NetworkManager::InitCallback callback)
	{
		m_Server.Initialize();

		m_Server.AddPacketListener(PacketType::AddressPairResponse, [this, callback = std::move(callback)](ReceivedPacket& packet)
		{
			{
				std::scoped_lock<std::mutex> lock(m_InitializedMutex);
				AddressPairResponsePacket response;
				Deserialize(packet.Packet, response);
				m_Address = response.Address;
				m_IsInitialized = true;
				BLT_INFO("NetworkManager Initialized with Public={0}, Private={1}", m_Address.PublicEndpoint, m_Address.PrivateEndpoint);
			}
			callback();
			return true;
		});

		UploadAddressPacket addressPacket;
		addressPacket.PrivateEndpoint = m_Server.Address();
		m_Server.SendPacket(s_Ec2Address, addressPacket);
	}

	void NetworkManager::Terminate(NetworkManager::TerminateCallback callback)
	{
		std::scoped_lock<std::mutex> lock(m_InitializedMutex);
		m_IsInitialized = false;
		m_Server.Terminate(std::move(callback));
	}

}