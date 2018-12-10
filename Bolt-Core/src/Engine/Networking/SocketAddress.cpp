#include "Types.h"

#include "SocketAddress.h"

namespace Bolt
{

	SocketAddress::SocketAddress(uint32_t inAddress, uint16_t inPort)
	{
		GetAsSockAddrIn()->sin_family = AF_INET;
		GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(inAddress);
		GetAsSockAddrIn()->sin_port = htons(inPort);
	}

	SocketAddress::SocketAddress(const blt::string& inAddress, const blt::string& inPort)
	{
		GetAsSockAddrIn()->sin_family = AF_INET;

		addrinfo hint;
		memset(&hint, 0, sizeof(addrinfo));
		hint.ai_family = AF_INET;

		addrinfo* result = new addrinfo();
		int error = getaddrinfo(inAddress.c_str(), inPort.c_str(), &hint, &result);
		if (error != 0 && result != nullptr)
		{
			freeaddrinfo(result);
			BLT_ASSERT(false, "Socket Error");
		}
		else if (error != 0)
		{
			BLT_ASSERT(false, "Socket Error");
		}
		while (!result->ai_addr && result->ai_next)
		{
			result = result->ai_next;
		}
		if (!result->ai_addr)
		{
			freeaddrinfo(result);
			BLT_ASSERT(false, "Socket Error");
		}

		memcpy(&m_SockAddr, result->ai_addr, sizeof(sockaddr));

		freeaddrinfo(result);
	}

	SocketAddress::SocketAddress(const blt::string& inAddress, uint16_t inPort) : SocketAddress(inAddress, std::to_string(inPort))
	{

	}

	SocketAddress::SocketAddress(const sockaddr& inSockAddr)
	{
		memcpy(&m_SockAddr, &inSockAddr, sizeof(sockaddr));
	}

	SocketAddress::SocketAddress() : SocketAddress(sockaddr())
	{

	}

	size_t SocketAddress::GetSize()
	{
		return sizeof(sockaddr);
	}

	bool SocketAddress::operator==(const SocketAddress& other) const
	{
		BLT_CORE_INFO(m_SockAddr.sa_data == other.m_SockAddr.sa_data);
		return (m_SockAddr.sa_data == other.m_SockAddr.sa_data) && (m_SockAddr.sa_family == other.m_SockAddr.sa_family);
	}

	bool SocketAddress::operator!=(const SocketAddress& other) const
	{
		return !(*this == other);
	}

	sockaddr_in* SocketAddress::GetAsSockAddrIn()
	{
		return reinterpret_cast<sockaddr_in*>(&m_SockAddr);
	}

}