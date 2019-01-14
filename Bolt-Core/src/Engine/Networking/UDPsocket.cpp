#include "Types.h"
#include "UDPsocket.h"

namespace Bolt
{

	UDPsocket::UDPsocket(SOCKET socket) : m_Socket(socket)
	{
	
	}

	UDPsocket::UDPsocket(AddressFamily addressFamily) : UDPsocket(socket((int)addressFamily, SOCK_DGRAM, IPPROTO_UDP))
	{
		BLT_ASSERT(IsValid(), "Failed to create UDP socket");
	}

	UDPsocket::UDPsocket(UDPsocket&& other)
		: m_Socket(other.m_Socket)
	{
		other.m_Socket = INVALID_SOCKET;
	}

	UDPsocket& UDPsocket::operator=(UDPsocket&& other)
	{
		SOCKET mySocket = m_Socket;
		m_Socket = other.m_Socket;
		other.m_Socket = mySocket;
		return *this;
	}

	UDPsocket::~UDPsocket()
	{
		if (IsValid())
		{
			closesocket(m_Socket);
		}
	}

	bool UDPsocket::IsValid() const
	{
		return m_Socket != INVALID_SOCKET;
	}

	int UDPsocket::Bind(const SocketAddress& address)
	{
		BLT_ASSERT(IsValid(), "Cannot Bind invalid Socket");
		int err = bind(m_Socket, &address.m_SockAddr, address.GetSize());
		if (err != NO_ERROR)
		{
			int errorCode = WSAGetLastError();
			BLT_CORE_ERROR("Socket Error: " + std::to_string(errorCode));
			return -errorCode;
		}
		return NO_ERROR;
	}

	int UDPsocket::SendTo(const SocketAddress& address, const void* data, uint length)
	{
		BLT_ASSERT(IsValid(), "Cannot SendTo invalid Socket");
		int bytes = sendto(m_Socket, (const char*)data, length, 0, &address.m_SockAddr, address.GetSize());
		if (bytes < 0)
		{
			int errorCode = WSAGetLastError();
			BLT_CORE_ERROR("Socket Error: " + std::to_string(errorCode));
			return -errorCode;
		}
		return bytes;
	}

	int UDPsocket::RecvFrom(void* buffer, uint length, SocketAddress* outAddress)
	{
		BLT_ASSERT(IsValid(), "Cannot RecvFrom invalid Socket");
		SocketAddress outAddr;
		int fromLength;
		int bytes = recvfrom(m_Socket, (char*)buffer, length, 0, (outAddress == nullptr) ? &outAddr.m_SockAddr : &outAddress->m_SockAddr, &fromLength);
		BLT_ASSERT(fromLength == outAddr.GetSize(), "Size mismatch error");
		if (bytes < 0)
		{
			int errorCode = WSAGetLastError();
			BLT_CORE_ERROR("Socket Error: " + std::to_string(errorCode));
			return -errorCode;
		}
		return bytes;
	}

	int UDPsocket::Close()
	{
		BLT_ASSERT(IsValid(), "Cannot Close invalid Socket");
		int err = closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
		if (err != NO_ERROR)
		{
			int errorCode = WSAGetLastError();
			BLT_CORE_ERROR("Socket Close Error: " + std::to_string(errorCode));
		}
		return err;
	}

}