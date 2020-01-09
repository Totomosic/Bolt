#include "bltpch.h"
#include "TCPsocket.h"

namespace Bolt
{

#ifndef BLT_PLATFORM_WINDOWS

	TCPsocket::TCPsocket(SocketHandle socket)
		: m_Socket(socket)
	{

	}

	TCPsocket::TCPsocket(AddressFamily addressFamily) : TCPsocket(socket((int)addressFamily, SOCK_STREAM, IPPROTO_TCP))
	{
		BLT_ASSERT(IsValid(), "Failed to create TCP socket");
	}

	TCPsocket::TCPsocket(TCPsocket&& other)
		: m_Socket(other.m_Socket)
	{
		other.m_Socket = BLT_INVALID_SOCKET;
	}

	TCPsocket& TCPsocket::operator=(TCPsocket&& other)
	{
		SocketHandle sock = m_Socket;
		m_Socket = other.m_Socket;
		other.m_Socket = sock;
		return *this;
	}

	TCPsocket::~TCPsocket()
	{
		if (IsValid())
		{
			Shutdown();
			Close();
		}
	}

	bool TCPsocket::IsValid() const
	{
		return m_Socket != BLT_INVALID_SOCKET;
	}

	int TCPsocket::Bind(const SocketAddress& address)
	{
		BLT_ASSERT(IsValid(), "Cannot Bind invalid Socket");
		int err = bind(m_Socket, &address.m_SockAddr, address.GetSize());
		return NO_ERROR;
	}

	int TCPsocket::Listen(int backlog)
	{
		BLT_ASSERT(IsValid(), "Cannot Listen on invalid Socket");
		int err = listen(m_Socket, backlog);
		return NO_ERROR;
	}

	TCPsocket TCPsocket::Accept(SocketAddress* outAddress)
	{
		BLT_ASSERT(IsValid(), "Cannot Accept on invalid Socket");
		socklen_t length = SocketAddress::GetSize();
		SocketHandle newSocket = BLT_INVALID_SOCKET;
		if (outAddress != nullptr)
		{
			newSocket = accept(m_Socket, &outAddress->m_SockAddr, &length);
		}
		else
		{
			SocketAddress addr;
			newSocket = accept(m_Socket, &addr.m_SockAddr, &length);
		}
		if (newSocket != BLT_INVALID_SOCKET)
		{
			return TCPsocket(newSocket);
		}
		BLT_CORE_ERROR("Socket Accept Error");
		return TCPsocket(BLT_INVALID_SOCKET);
	}

	int TCPsocket::Connect(const SocketAddress& address)
	{
		BLT_ASSERT(IsValid(), "Cannot Connect invalid Socket");
		int err = connect(m_Socket, &address.m_SockAddr, address.GetSize());
		return NO_ERROR;
	}

	int TCPsocket::Send(const void* data, uint32_t length)
	{
		BLT_ASSERT(IsValid(), "Cannot Send with invalid Socket");
		int bytesSent = send(m_Socket, static_cast<const char*>(data), (int)length, 0);
		return bytesSent;
	}

	int TCPsocket::Recv(void* buffer, uint32_t length)
	{
		BLT_ASSERT(IsValid(), "Cannot Recv from invalid Socket");
		int bytesReceived = recv(m_Socket, static_cast<char*>(buffer), (int)length, 0);
		return bytesReceived;
	}

	int TCPsocket::Shutdown()
	{
		BLT_ASSERT(IsValid(), "Cannot Shutdown invalid Socket");
		int err = shutdown(m_Socket, SHUT_RDWR);
		return err;
	}

	int TCPsocket::Close()
	{
		BLT_ASSERT(IsValid(), "Cannot Close invalid Socket");
		int err = close(m_Socket);
		m_Socket = BLT_INVALID_SOCKET;
		return err;
	}

#endif

}