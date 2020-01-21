#pragma once
#include "bltpch.h"

namespace Bolt
{

	BLT_API enum class AddressFamily
	{
		INET = AF_INET,
		INET6 = AF_INET6
	};

#ifdef BLT_PLATFORM_WINDOWS
	using SocketHandle = SOCKET;
#define BLT_INVALID_SOCKET INVALID_SOCKET
#else
	using SocketHandle = int;
#define BLT_INVALID_SOCKET 0
#define NO_ERROR 0
#endif

}