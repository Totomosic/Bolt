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
		size_t GetHash() const;
		blt::string ToString() const;

		bool operator==(const SocketAddress& other) const;
		bool operator!=(const SocketAddress& other) const;

		friend std::ostream& operator<<(std::ostream& stream, const SocketAddress& address);

		friend class SocketUtil;

	private:
		const sockaddr_in* GetAsSockAddrIn() const;
		sockaddr_in* GetAsSockAddrIn();
		const uint& GetIP4Ref() const;
		uint& GetIP4Ref();

	};

}

namespace std
{

	template<>
	struct hash<Bolt::SocketAddress>
	{
		size_t operator()(const Bolt::SocketAddress& value) const
		{
			return value.GetHash();
		}
	};

}