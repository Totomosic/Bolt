#include "bltpch.h"
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
#ifdef BLT_PLATFORM_WINDOWS
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
#endif
		return result;
	}

	UDPsocket* SocketUtil::SelectRead(UDPsocket* socket, int timeoutMilliseconds)
	{
		fd_set readSet;
		FD_ZERO(&readSet);
		FD_SET(socket->m_Socket, &readSet);
		timeval t;
		t.tv_sec = timeoutMilliseconds / 1000;
		t.tv_usec = timeoutMilliseconds * 1000 - t.tv_sec * 1000000;
		if (select(1, &readSet, nullptr, nullptr, &t) > 0)
		{
			if (FD_ISSET(socket->m_Socket, &readSet))
			{
				return socket;
			}
		}
		return nullptr;
	}

}