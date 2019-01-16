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

		static int Select(const UDPsocketSet& inReadSet, UDPsocketSet* outReadSet,
			const UDPsocketSet& inWriteSet, UDPsocketSet* outWriteSet,
			const UDPsocketSet& inErrorSet, UDPsocketSet* outErrorSet, int timeOutMilliseconds = -1);

		static int Select(const UDPsocketSet& inReadSet, UDPsocketSet* outReadSet,
			const UDPsocketSet& inWriteSet, UDPsocketSet* outWriteSet, int timeOutMilliseconds = -1);

		static int Select(const UDPsocketSet& inReadSet, UDPsocketSet* outReadSet, int timeOutMilliseconds = -1);

	private:
		static fd_set* FillSetFromVector(fd_set& set, const UDPsocketSet& sockets);
		static void FillVectorFromSet(UDPsocketSet& outSockets, const UDPsocketSet& inSockets, const fd_set& set);

	};

}