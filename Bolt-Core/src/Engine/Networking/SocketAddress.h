#pragma once
#include "Networking.h"

namespace Bolt
{

	struct BLT_API SocketAddress
	{
	public:
		sockaddr m_SockAddr;

	public:
		SocketAddress(uint32_t inAddress, uint16_t inPort);
		SocketAddress(const blt::string& inAddress, const blt::string& inPort);
		SocketAddress(const blt::string& inAddress, uint16_t inPort);
		SocketAddress(const sockaddr& inSockAddr);
		SocketAddress();

		static size_t GetSize();

		bool operator==(const SocketAddress& other) const;
		bool operator!=(const SocketAddress& other) const;

	private:
		sockaddr_in* GetAsSockAddrIn();

	};

}