#include "bltpch.h"
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
		addrinfo hint;
		memset(&hint, 0, sizeof(addrinfo));
		hint.ai_family = AF_INET;

		addrinfo* result = new addrinfo();
		int error = getaddrinfo(inAddress.c_str(), inPort.c_str(), &hint, &result);
		if (error != 0 && result != nullptr)
		{
			BLT_ERROR("SocketAddress Hostname Error");
		}
		else if (error != 0)
		{
			BLT_ERROR("SocketAddress Hostname Error");
		}
		else
		{
			while (!result->ai_addr && result->ai_next)
			{
				result = result->ai_next;
			}
			if (!result->ai_addr)
			{
				BLT_ERROR("SocketAddress Hostname Error");
			}
			memcpy(&m_SockAddr, result->ai_addr, sizeof(sockaddr));
			GetAsSockAddrIn()->sin_family = AF_INET;
		}		
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

	uint32_t SocketAddress::GetSize()
	{
		return sizeof(sockaddr);
	}

	uint32_t SocketAddress::GetHash() const
	{
		return (GetIP4Ref()) | ((static_cast<uint32_t>(GetAsSockAddrIn()->sin_port)) << 13) | m_SockAddr.sa_family;
	}

	blt::string SocketAddress::ToString() const
	{
		const sockaddr_in* s = GetAsSockAddrIn();
		uint16_t port = ntohs(s->sin_port);
		uint32_t addr = ((GetAsSockAddrIn()->sin_addr.S_un.S_addr));
		byte* bytes = (byte*)&addr;
		return std::to_string((int)bytes[0]) + "." + std::to_string((int)bytes[1]) + "." + std::to_string((int)bytes[2]) + "." + std::to_string((int)bytes[3]) + ":" + std::to_string(port);
	}

	bool SocketAddress::operator==(const SocketAddress& other) const
	{
		return (GetAsSockAddrIn()->sin_port == other.GetAsSockAddrIn()->sin_port) && (GetIP4Ref() == other.GetIP4Ref());
	}

	bool SocketAddress::operator!=(const SocketAddress& other) const
	{
		return !(*this == other);
	}

	std::ostream& operator<<(std::ostream& stream, const SocketAddress& address)
	{
		stream << address.ToString();
		return stream;
	}

	const sockaddr_in* SocketAddress::GetAsSockAddrIn() const
	{
		return reinterpret_cast<const sockaddr_in*>(&m_SockAddr);
	}

	sockaddr_in* SocketAddress::GetAsSockAddrIn()
	{
		return reinterpret_cast<sockaddr_in*>(&m_SockAddr);
	}

	const uint32_t& SocketAddress::GetIP4Ref() const
	{
		return *reinterpret_cast<const uint32_t*>(&GetAsSockAddrIn()->sin_addr.S_un.S_addr);
	}

	uint32_t& SocketAddress::GetIP4Ref()
	{
		return *reinterpret_cast<uint32_t*>(&GetAsSockAddrIn()->sin_addr.S_un.S_addr);
	}

}