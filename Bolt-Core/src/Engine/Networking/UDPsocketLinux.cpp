#include "bltpch.h"
#include "UDPsocket.h"

namespace Bolt
{

#ifndef BLT_PLATFORM_WINDOWS

	UDPsocket::UDPsocket(SocketHandle socket) 
		: m_Socket(socket)
	{

	}

	UDPsocket::UDPsocket(AddressFamily addressFamily) : UDPsocket(socket((int)addressFamily, SOCK_DGRAM, IPPROTO_UDP))
	{
		BLT_ASSERT(IsValid(), "Failed to create UDP socket");
	}

	UDPsocket::UDPsocket(UDPsocket&& other)
		: m_Socket(other.m_Socket)
	{
		other.m_Socket = BLT_INVALID_SOCKET;
	}

	UDPsocket& UDPsocket::operator=(UDPsocket&& other)
	{
		SocketHandle mySocket = m_Socket;
		m_Socket = other.m_Socket;
		other.m_Socket = mySocket;
		return *this;
	}

	UDPsocket::~UDPsocket()
	{
		if (IsValid())
		{
			Shutdown();
			Close();
		}
	}

	bool UDPsocket::IsValid() const
	{
		return m_Socket != BLT_INVALID_SOCKET;
	}

	int UDPsocket::Bind(const SocketAddress& address)
	{
		BLT_ASSERT(IsValid(), "Cannot Bind invalid Socket");
		int err = bind(m_Socket, &address.m_SockAddr, address.GetSize());
		if (err != 0)
		{
			BLT_CORE_ERROR("Socket Bind Failed");
		}
		return err;
	}

	int UDPsocket::SendTo(const SocketAddress& address, const void* data, uint32_t length)
	{
		BLT_ASSERT(IsValid(), "Cannot SendTo invalid Socket");
		int bytes = sendto(m_Socket, (const char*)data, length, 0, &address.m_SockAddr, address.GetSize());
		return bytes;
	}

	int UDPsocket::RecvFrom(void* buffer, uint32_t length, SocketAddress* outAddress)
	{
		BLT_ASSERT(IsValid(), "Cannot RecvFrom invalid Socket");
		SocketAddress outAddr;
		socklen_t fromLength = outAddr.GetSize();
		int bytes = recvfrom(m_Socket, (char*)buffer, length, 0, (outAddress == nullptr) ? &outAddr.m_SockAddr : &outAddress->m_SockAddr, &fromLength);
		BLT_ASSERT(fromLength == outAddr.GetSize(), "Size mismatch error");
		return bytes;
	}

	int UDPsocket::Shutdown()
	{
		BLT_ASSERT(IsValid(), "Cannot Shutdown invalid Socket");
		int err = shutdown(m_Socket, SHUT_RDWR);
		return err;
	}

	int UDPsocket::Close()
	{
		BLT_ASSERT(IsValid(), "Cannot Close invalid Socket");
		int err = close(m_Socket);
		m_Socket = BLT_INVALID_SOCKET;
		return err;
	}

	void UDPsocket::SetBlocking(bool isBlocking)
	{
		
	}

#endif

}