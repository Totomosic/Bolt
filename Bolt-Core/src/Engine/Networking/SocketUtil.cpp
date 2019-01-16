#include "Types.h"
#include "SocketUtil.h"

namespace Bolt
{

	int SocketUtil::Select(const UDPsocketSet& inReadSet, UDPsocketSet* outReadSet,
		const UDPsocketSet& inWriteSet, UDPsocketSet* outWriteSet,
		const UDPsocketSet& inErrorSet, UDPsocketSet* outErrorSet, int timeoutMilliseconds)
	{
		fd_set read;
		fd_set write;
		fd_set error;

		fd_set* readPtr = FillSetFromVector(read, inReadSet);
		fd_set* writePtr = FillSetFromVector(write, inWriteSet);
		fd_set* errorPtr = FillSetFromVector(error, inErrorSet);

		timeval timeout;
		timeval* timeoutPtr = nullptr;
		if (timeoutMilliseconds >= 0)
		{
			timeout.tv_sec = timeoutMilliseconds / 1000;
			timeout.tv_usec = (timeoutMilliseconds % 1000) * 1000;
		}

		int result = select(0, readPtr, writePtr, errorPtr, timeoutPtr);
		if (result > 0)
		{
			FillVectorFromSet(*outReadSet, inReadSet, read);
			FillVectorFromSet(*outWriteSet, inWriteSet, write);
			FillVectorFromSet(*outErrorSet, inErrorSet, error);
		}
		return result;
	}

	int SocketUtil::Select(const UDPsocketSet& inReadSet, UDPsocketSet* outReadSet,
		const UDPsocketSet& inWriteSet, UDPsocketSet* outWriteSet, int timeoutMilliseconds)
	{
		UDPsocketSet error;
		UDPsocketSet outError;
		return Select(inReadSet, outReadSet, inWriteSet, outWriteSet, error, &outError, timeoutMilliseconds);
	}

	int SocketUtil::Select(const UDPsocketSet& inReadSet, UDPsocketSet* outReadSet, int timeoutMilliseconds)
	{
		UDPsocketSet write;
		UDPsocketSet outWrite;
		return Select(inReadSet, outReadSet, write, &outWrite, timeoutMilliseconds);
	}

	fd_set* SocketUtil::FillSetFromVector(fd_set& set, const UDPsocketSet& sockets)
	{
		FD_ZERO(&set);
		for (UDPsocket* socket : sockets)
		{
			FD_SET(socket->m_Socket, &set);
		}
		return &set;
	}

	void SocketUtil::FillVectorFromSet(UDPsocketSet& outSockets, const UDPsocketSet& inSockets, const fd_set& set)
	{
		outSockets.clear();
		for (UDPsocket* socket : inSockets)
		{
			if (FD_ISSET(socket->m_Socket, &set))
			{
				outSockets.push_back(socket);
			}
		}
	}

}