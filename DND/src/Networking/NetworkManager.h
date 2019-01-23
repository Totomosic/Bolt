#pragma once
#include "NetworkServer.h"
#include "ConnectionManager.h"

namespace DND
{

	class NetworkManager
	{
	public:
		using OnInitCallback = std::function<void(const AddressPair&)>;
		using OnExitCallback = std::function<void()>;

		using OnGetHostsCallback = std::function<void(std::vector<AddressPair>)>;
		using OnConnectedCallback = std::function<void(id_t)>;

	public:
		static SocketAddress EC2_SERVER_ADDRESS;

	private:
		AddressPair m_Address;
		NetworkServer m_Server;
		ConnectionManager m_Connections;

	public:
		NetworkManager();

		inline const NetworkServer& Server() const { return m_Server; }
		inline NetworkServer& Server() { return m_Server; }

		const AddressPair& Address() const;
		void SetAddress(const AddressPair& address);

		void Initialize(const SocketAddress& bindAddress, OnInitCallback callback);
		void Exit(OnExitCallback callback);
		void GetAllHosts(OnGetHostsCallback callback);

		void ConnectTo(const AddressPair& address, OnConnectedCallback callback);
		void ConnectToWithoutServer(const AddressPair& address, OnConnectedCallback callback);
		void DisconnectFrom(id_t connectionId);		

	};

}