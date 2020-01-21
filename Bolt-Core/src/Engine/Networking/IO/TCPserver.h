#pragma once
#include "TCPclient.h"

namespace Bolt
{

	struct BLT_API TCPconnected
	{
	public:
		TCPclient& Client;
		SocketAddress Address;
	};

	class BLT_API TCPserver
	{
	private:
		std::mutex m_ClientMutex;
		TCPsocket m_Socket;
		GenericEventBus<std::string> m_Events;
		std::vector<std::unique_ptr<TCPclient>> m_Clients;

	public:
		TCPserver();

		const GenericEventBus<std::string>& Bus() const;
		GenericEventBus<std::string>& Bus();

		void Listen(const SocketAddress& address, int backlog);

		void Broadcast(const std::string& type, const void* data, uint32_t length);
		void Broadcast(const std::string& type, const std::string& data);
		void Broadcast(const std::string& type);

	};

}