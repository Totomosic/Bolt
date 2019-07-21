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
		GenericEventBus<blt::string> m_Events;
		std::vector<std::unique_ptr<TCPclient>> m_Clients;

	public:
		TCPserver();

		const GenericEventBus<blt::string>& Bus() const;
		GenericEventBus<blt::string>& Bus();

		void Listen(const SocketAddress& address, int backlog);

		void Broadcast(const blt::string& type, const void* data, uint32_t length);
		void Broadcast(const blt::string& type, const blt::string& data);
		void Broadcast(const blt::string& type);

	};

}