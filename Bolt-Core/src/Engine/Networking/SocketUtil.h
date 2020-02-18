#pragma once
#include "TCPsocket.h"
#include "UDPsocket.h"

namespace Bolt
{

	class BLT_API SocketUtil
	{
	public:
		using TCPsocketSet = std::vector<TCPsocket*>;
		using UDPsocketSet = std::vector<UDPsocket*>;

	public:
		SocketUtil() = delete;

		static bool IsBigEndian();
		static bool IsOnSameSubnet(const SocketAddress& a, const SocketAddress& b, int sigBits);

		static std::vector<uint32_t> GetIP4Addresses();

		static UDPsocket* SelectRead(UDPsocket* socket, int timeoutMilliseconds = 0);

	};

}