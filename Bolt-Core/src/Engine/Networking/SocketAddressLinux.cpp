#include "bltpch.h"
#include "SocketAddress.h"

#ifndef BLT_PLATFORM_WINDOWS

#include <netdb.h>
#include <arpa/inet.h>

#endif

namespace Bolt
{

#ifndef BLT_PLATFORM_WINDOWS

	SocketAddress::SocketAddress(uint32_t inAddress, uint16_t inPort)
		: m_SockAddr()
	{
		GetAsSockAddrIn()->sin_family = AF_INET;
		GetAsSockAddrIn()->sin_addr.s_addr = htonl(inAddress);
		GetAsSockAddrIn()->sin_port = htons(inPort);
	}

	SocketAddress::SocketAddress(byte b0, byte b1, byte b2, byte b3, uint16_t inPort) : SocketAddress(CreateAddress(b0, b1, b2, b3), inPort)
	{
	}

	SocketAddress::SocketAddress(const std::string& inAddress, const std::string& inPort) : SocketAddress(inAddress, std::stoi(inPort.c_str()))
	{
		
	}

	SocketAddress::SocketAddress(const std::string& inAddress, uint16_t inPort)
		: m_SockAddr()
	{
		GetAsSockAddrIn()->sin_family = AF_INET;
		GetAsSockAddrIn()->sin_port = htons(inPort);

		struct hostent* he;
		struct in_addr** addr_list;

		if ((he = gethostbyname(inAddress.c_str())) == NULL)
		{
			BLT_CORE_ERROR("Get host failed for hostname {}", inAddress);
		}
		else
		{
			addr_list = (struct in_addr**)he->h_addr_list;
			struct in_addr* addr = nullptr;
			for (int i = 0; addr_list[i] != nullptr; i++)
			{
				addr = addr_list[i];
				break;
			}
			GetAsSockAddrIn()->sin_addr = *addr;
		}
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

	std::string SocketAddress::ToString() const
	{
		const sockaddr_in* s = GetAsSockAddrIn();
		uint16_t port = ntohs(s->sin_port);
		uint32_t addr = ((GetAsSockAddrIn()->sin_addr.s_addr));
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
		return *reinterpret_cast<const uint32_t*>(&GetAsSockAddrIn()->sin_addr.s_addr);
	}

	uint32_t& SocketAddress::GetIP4Ref()
	{
		return *reinterpret_cast<uint32_t*>(&GetAsSockAddrIn()->sin_addr.s_addr);
	}

	uint32_t SocketAddress::CreateAddress(byte b0, byte b1, byte b2, byte b3)
	{
		struct in_addr addr;
		unsigned char* ip;

		//have ip point to s_addr
		ip = (unsigned char*)&(addr.s_addr);

		//set the bytes for "10.4.32.41"
		ip[0] = b3;
		ip[1] = b2;
		ip[2] = b1;
		ip[3] = b0;
		return addr.s_addr;
	}

#endif

}