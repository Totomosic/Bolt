#pragma once
#include "Networking.h"
#include "SocketAddress.h"

namespace Bolt
{

	class BLT_API TCPsocket
	{
	private:
		SocketHandle m_Socket;

	public:
		TCPsocket(SocketHandle socket);
		TCPsocket(AddressFamily addressFamily = AddressFamily::INET);
		TCPsocket(const TCPsocket& other) = delete;
		TCPsocket& operator=(const TCPsocket& other) = delete;
		TCPsocket(TCPsocket&& other);
		TCPsocket& operator=(TCPsocket&& other);
		~TCPsocket();

		bool IsValid() const;

		int Bind(const SocketAddress& address);
		int Listen(int backlog);
		TCPsocket Accept(SocketAddress* outAddress);
		int Connect(const SocketAddress& address);
		int Send(const void* data, uint32_t length);
		int Recv(void* buffer, uint32_t length);
		int Shutdown();
		int Close();

		friend class SocketUtil;

	};

}