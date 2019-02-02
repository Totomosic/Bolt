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

		const AddressPair& Address() const;
		bool IsInitialized() const;

		void Initialize(InitCallback callback);
		void Terminate(TerminateCallback callback);

		void RegisterAsHost();
		void RemoveAsHost();
		void GetAllHosts(float timeout, GetHostsCallback callback, GetHostsTimeoutCallback timeoutCallback);

		void Update();

	};

}