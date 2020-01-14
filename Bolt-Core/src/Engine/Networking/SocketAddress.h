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
		SocketAddress(byte b0, byte b1, byte b2, byte b3, uint16_t inPort);
		SocketAddress(const std::string& inAddress, const std::string& inPort);
		SocketAddress(const std::string& inAddress, uint16_t inPort);
		SocketAddress(const sockaddr& inSockAddr);
		SocketAddress();

		static uint32_t GetSize();
		uint32_t GetHash() const;
		std::string ToString() const;

		bool operator==(const SocketAddress& other) const;
		bool operator!=(const SocketAddress& other) const;

		friend std::ostream& operator<<(std::ostream& stream, const SocketAddress& address);

		friend class SocketUtil;

	private:
		const sockaddr_in* GetAsSockAddrIn() const;
		sockaddr_in* GetAsSockAddrIn();
		const uint32_t& GetIP4Ref() const;
		uint32_t& GetIP4Ref();
		static uint32_t CreateAddress(byte b0, byte b1, byte b2, byte b3);

	};

}

namespace std
{

	template<>
	struct hash<Bolt::SocketAddress>
	{
		uint32_t operator()(const Bolt::SocketAddress& value) const
		{
			return value.GetHash();
		}
	};

}