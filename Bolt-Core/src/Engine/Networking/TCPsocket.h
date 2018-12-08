#pragma once
#include "Networking.h"
#include "SocketAddress.h"

namespace Bolt
{

	class BLT_API TCPsocket
	{
	private:
		SOCKET m_Socket;

	public:
		TCPsocket(SOCKET socket);
		TCPsocket(AddressFamily addressFamily = AddressFamily::INET);
		TCPsocket(const TCPsocket& other) = delete;
		TCPsocket& operator=(const TCPsocket& other) = delete;
		TCPsocket(TCPsocket&& other);
		TCPsocket& operator=(TCPsocket&& other);
		~TCPsocket();

		int Bind(const SocketAddress& address);
		int Listen(int backlog);
		TCPsocket Accept(SocketAddress* outAddress);
		int Connect(const SocketAddress& address);
		int Send(const void* data, uint length);
		int Recv(void* buffer, uint length);
		int Close();

	};

}