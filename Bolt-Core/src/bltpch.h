#pragma once
#include "Core/Memory/Allocator.h"
#include "freetype-gl.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "lua.hpp"

#include "Core/BoltTL/string.h"
#include "Core/BoltTL/array2d.h"
#include "BoltDefines.h"

#include <algorithm>
#include <any>
#include <bitset>
#include <chrono>
#include <cstdlib>
#include <deque>

#ifdef BLT_PLATFORM_WINDOWS
#include <direct.h>
#endif

#include <fstream>
#include <functional>
#include <future>
#include <ios>
#include <iostream>
#include <memory>
#include <mutex>
#include <numeric>
#include <optional>
#include <ratio>
#include <sstream>
#include <stdio.h>
#include <string>
#include <thread>
#include <typeindex>
#include <typeinfo>
#include <type_traits>

#ifdef BLT_PLATFORM_WINDOWS
#define SCK_VERSION 0x0202
#include <WinSock2.h>
#include <WS2tcpip.h>
#elif BLT_PLATFORM_LINUX
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

namespace Bolt
{

	using byte = uint8_t;
	using id_t = uint32_t;

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

}