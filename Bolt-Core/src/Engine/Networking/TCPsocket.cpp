#include "TCPsocket.h"

namespace Bolt
{

	TCPsocket::TCPsocket(SOCKET socket)
		: m_Socket(socket)
	{
	
	}

	TCPsocket::TCPsocket(AddressFamily addressFamily) : TCPsocket(socket((int)addressFamily, SOCK_STREAM, IPPROTO_TCP))
	{
	
	}

	TCPsocket::TCPsocket(TCPsocket&& other)
		: m_Socket(other.m_Socket)
	{
		other.m_Socket = INVALID_SOCKET;
	}

	TCPsocket& TCPsocket::operator=(TCPsocket&& other)
	{
		m_Socket = other.m_Socket;
		other.m_Socket = m_Socket;
		return *this;
	}

	TCPsocket::~TCPsocket()
	{
		if (m_Socket != INVALID_SOCKET)
		{
			closesocket(m_Socket);
		}
	}

	int TCPsocket::Bind(const SocketAddress& address)
	{
		int err = bind(m_Socket, &address.m_SockAddr, address.GetSize());
		if (err < 0)
		{
			int errorCode = WSAGetLastError();
			BLT_CORE_ERROR("Socket Error: " + std::to_string(errorCode));
			return errorCode;
		}
		return NO_ERROR;
	}

	int TCPsocket::Listen(int backlog)
	{
		int err = listen(m_Socket, backlog);
		if (err != NO_ERROR)
		{
			int errorCode = WSAGetLastError();
			BLT_CORE_ERROR("Socket Error: " + std::to_string(errorCode));
			return -errorCode;
		}
		return NO_ERROR;
	}

	TCPsocket TCPsocket::Accept(SocketAddress* outAddress)
	{
		int length = SocketAddress::GetSize();
		SOCKET newSocket = INVALID_SOCKET;
		if (outAddress != nullptr)
		{
			newSocket = accept(m_Socket, &outAddress->m_SockAddr, &length);
		}
		else
		{
			SocketAddress* temp = &SocketAddress();
			newSocket = accept(m_Socket, &temp->m_SockAddr, &length);
		}
		if (newSocket != INVALID_SOCKET)
		{
			return TCPsocket(newSocket);
		}
		BLT_CORE_ERROR("Socket Error");
		return TCPsocket(INVALID_SOCKET);
	}

	int TCPsocket::Connect(const SocketAddress& address)
	{
		int err = connect(m_Socket, &address.m_SockAddr, address.GetSize());
		if (err < 0)
		{
			int res = WSAGetLastError();
			BLT_CORE_ERROR("Socket Error: " + std::to_string(res));
			return -res;
		}
		return NO_ERROR;
	}

	int TCPsocket::Send(const void* data, uint length)
	{
		int bytesSent = send(m_Socket, static_cast<const char*>(data), (int)length, 0);
		if (bytesSent < 0)
		{
			int error = WSAGetLastError();
			BLT_CORE_ERROR("Socket send error");
			return -error;
		}
		return bytesSent;
	}

	int TCPsocket::Recv(void* buffer, uint length)
	{
		int bytesReceived = recv(m_Socket, static_cast<char*>(buffer), (int)length, 0);
		if (bytesReceived < 0)
		{
			int error = WSAGetLastError();
			BLT_CORE_ERROR("Socket recv error: " + std::to_string(error));
			return -error;
		}
		return bytesReceived;
	}

}