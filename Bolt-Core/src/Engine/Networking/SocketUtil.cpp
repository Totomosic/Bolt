#include "Types.h"
#include "SocketUtil.h"

namespace Bolt
{

	bool SocketUtil::IsBigEndian()
	{
		return htonl(47) == 47;
	}

	bool SocketUtil::IsOnSameSubnet(const SocketAddress& a, const SocketAddress& b, int sigBits)
	{
		id_t mask = 0;
		for (int i = 0; i < sigBits; i++)
		{
			mask += 1 << (i);
		}
		id_t amask = a.GetIP4Ref() & mask;
		id_t bmask = b.GetIP4Ref() & mask;
		bool result = amask == bmask;
		BLT_CORE_INFO("COMPARING {0} and {1} RESULT {2}", a.ToString(), b.ToString(), result);
		return result;
	}

	std::vector<uint32_t> SocketUtil::GetIP4Addresses()
	{
		std::vector<uint32_t> result;
		char name[255];
		if (gethostname(name, sizeof(name)) == 0)
		{
			PHOSTENT hostinfo;
			if ((hostinfo = gethostbyname(name)) != NULL)
			{
				int nCount = 0;
				while (hostinfo->h_addr_list[nCount])
				{
					in_addr addr = *(struct in_addr*)hostinfo->h_addr_list[nCount++];
					uint32_t a = ntohl(addr.S_un.S_addr);
					result.push_back(a);
				}
			}
		}
		return result;
	}

	int SocketUtil::Select(const UDPsocketSet* inReadSet, UDPsocketSet* outReadSet,
		const UDPsocketSet* inWriteSet, UDPsocketSet* outWriteSet,
		const UDPsocketSet* inErrorSet, UDPsocketSet* outErrorSet, int timeoutMilliseconds)
	{
		fd_set read;
		fd_set write;
		fd_set error;

		fd_set* readPtr = (inReadSet != nullptr) ? FillSetFromVector(read, *inReadSet) : nullptr;
		fd_set* writePtr = (inWriteSet != nullptr) ? FillSetFromVector(write, *inWriteSet) : nullptr;
		fd_set* errorPtr = (inErrorSet != nullptr) ? FillSetFromVector(error, *inErrorSet) : nullptr;

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
			FillVectorFromSet(*outReadSet, *inReadSet, read);
			FillVectorFromSet(*outWriteSet, *inWriteSet, write);
			FillVectorFromSet(*outErrorSet, *inErrorSet, error);
		}
		return result;
	}

	int SocketUtil::Select(const UDPsocketSet* inReadSet, UDPsocketSet* outReadSet,
		const UDPsocketSet* inWriteSet, UDPsocketSet* outWriteSet, int timeoutMilliseconds)
	{
		return Select(inReadSet, outReadSet, inWriteSet, outWriteSet, nullptr, nullptr, timeoutMilliseconds);
	}

	int SocketUtil::Select(const UDPsocketSet* inReadSet, UDPsocketSet* outReadSet, int timeoutMilliseconds)
	{
		return Select(inReadSet, outReadSet, nullptr, nullptr, timeoutMilliseconds);
	}

	int SocketUtil::Select(const TCPsocketSet* inReadSet, TCPsocketSet* outReadSet,
		const TCPsocketSet* inWriteSet, TCPsocketSet* outWriteSet,
		const TCPsocketSet* inErrorSet, TCPsocketSet* outErrorSet, int timeoutMilliseconds)
	{
		fd_set read;
		fd_set write;
		fd_set error;

		fd_set* readPtr = (inReadSet != nullptr) ? FillSetFromVector(read, *inReadSet) : nullptr;
		fd_set* writePtr = (inWriteSet != nullptr) ? FillSetFromVector(write, *inWriteSet) : nullptr;
		fd_set* errorPtr = (inErrorSet != nullptr) ? FillSetFromVector(error, *inErrorSet) : nullptr;

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
			FillVectorFromSet(*outReadSet, *inReadSet, read);
			FillVectorFromSet(*outWriteSet, *inWriteSet, write);
			FillVectorFromSet(*outErrorSet, *inErrorSet, error);
		}
		return result;
	}

	int SocketUtil::Select(const TCPsocketSet* inReadSet, TCPsocketSet* outReadSet,
		const TCPsocketSet* inWriteSet, TCPsocketSet* outWriteSet, int timeoutMilliseconds)
	{
		return Select(inReadSet, outReadSet, inWriteSet, outWriteSet, nullptr, nullptr, timeoutMilliseconds);
	}

	int SocketUtil::Select(const TCPsocketSet* inReadSet, TCPsocketSet* outReadSet, int timeoutMilliseconds)
	{
		return Select(inReadSet, outReadSet, nullptr, nullptr, timeoutMilliseconds);
	}

	fd_set* SocketUtil::FillSetFromVector(fd_set& set, const UDPsocketSet& sockets)
	{
		if (&sockets == nullptr)
		{
			return &set;
		}
		FD_ZERO(&set);
		for (UDPsocket* socket : sockets)
		{
			FD_SET(socket->m_Socket, &set);
		}
		return &set;
	}

	void SocketUtil::FillVectorFromSet(UDPsocketSet& outSockets, const UDPsocketSet& inSockets, const fd_set& set)
	{
		if (&outSockets == nullptr || &inSockets == nullptr)
		{
			return;
		}
		outSockets.clear();
		for (UDPsocket* socket : inSockets)
		{
			if (FD_ISSET(socket->m_Socket, &set))
			{
				outSockets.push_back(socket);
			}
		}
	}

	fd_set* SocketUtil::FillSetFromVector(fd_set& set, const TCPsocketSet& sockets)
	{
		if (&sockets == nullptr)
		{
			return &set;
		}
		FD_ZERO(&set);
		for (TCPsocket* socket : sockets)
		{
			FD_SET(socket->m_Socket, &set);
		}
		return &set;
	}

	void SocketUtil::FillVectorFromSet(TCPsocketSet& outSockets, const TCPsocketSet& inSockets, const fd_set& set)
	{
		if (&outSockets == nullptr || &inSockets == nullptr)
		{
			return;
		}
		outSockets.clear();
		for (TCPsocket* socket : inSockets)
		{
			if (FD_ISSET(socket->m_Socket, &set))
			{
				outSockets.push_back(socket);
			}
		}
	}

}