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

		static int Select(const UDPsocketSet* inReadSet, UDPsocketSet* outReadSet,
			const UDPsocketSet* inWriteSet, UDPsocketSet* outWriteSet,
			const UDPsocketSet* inErrorSet, UDPsocketSet* outErrorSet, int timeOutMilliseconds = -1);
		static int Select(const UDPsocketSet* inReadSet, UDPsocketSet* outReadSet,
			const UDPsocketSet* inWriteSet, UDPsocketSet* outWriteSet, int timeOutMilliseconds = -1);
		static int Select(const UDPsocketSet* inReadSet, UDPsocketSet* outReadSet, int timeOutMilliseconds = -1);

		static int Select(const TCPsocketSet* inReadSet, TCPsocketSet* outReadSet,
			const TCPsocketSet* inWriteSet, TCPsocketSet* outWriteSet,
			const TCPsocketSet* inErrorSet, TCPsocketSet* outErrorSet, int timeOutMilliseconds = -1);
		static int Select(const TCPsocketSet* inReadSet, TCPsocketSet* outReadSet,
			const TCPsocketSet* inWriteSet, TCPsocketSet* outWriteSet, int timeOutMilliseconds = -1);
		static int Select(const TCPsocketSet* inReadSet, TCPsocketSet* outReadSet, int timeOutMilliseconds = -1);

	private:
		static fd_set* FillSetFromVector(fd_set& set, const UDPsocketSet& sockets);
		static void FillVectorFromSet(UDPsocketSet& outSockets, const UDPsocketSet& inSockets, const fd_set& set);

		static fd_set* FillSetFromVector(fd_set& set, const TCPsocketSet& sockets);
		static void FillVectorFromSet(TCPsocketSet& outSockets, const TCPsocketSet& inSockets, const fd_set& set);

	};

}