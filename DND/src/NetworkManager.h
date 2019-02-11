#pragma once
#include "Network/NetworkServer.h"
#include "Network/ServerInitPackets.h"

namespace DND
{

	class NetworkManager
	{
	private:
		static NetworkManager* s_Instance;
		static SocketAddress s_Ec2Address;

	public:
		static NetworkManager& Get();

		using InitCallback = std::function<void(bool)>;
		using TerminateCallback = std::function<void()>;
		using GetHostsCallback = std::function<void(std::vector<AddressPair>)>;
		using GetHostsTimeoutCallback = std::function<void()>;

	private:
		NetworkServer m_Server;
		
		mutable std::mutex m_InitializedMutex;
		AddressPair m_Address;
		bool m_IsInitialized;

	public:
		NetworkManager();

		NetworkServer& Server();
		const AddressPair& Address() const;
		bool IsInitialized() const;
		const SocketAddress& EC2Address() const;

		void Initialize(InitCallback callback);
		void Terminate(TerminateCallback callback);
		void Reconnect(InitCallback callback);

		void RegisterAsHost();
		void RemoveAsHost();
		void GetAllHosts(float timeout, GetHostsCallback callback, GetHostsTimeoutCallback timeoutCallback);

		void Update();

		template<typename T>
		void SendPacket(const SocketAddress& to, const T& packet)
		{
			m_Server.SendPacket(to, packet);
		}

	};

}