#pragma once
#include "Core/Memory/Allocator.h"
#include "freetype-gl.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "lua.hpp"

#include "BoltLib/Functions.h"
#include "BoltLib/BoltTL/string.h"
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

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	inline const char* ConvertError(uint32_t error)
	{
		switch (error)
		{
		case 0x0500: return "GL_INVALID_ENUM";
		case 0x0501: return "GL_INVALID_VALUE";
		case 0x0502: return "GL_INVALID_OPERATION";
		case 0x0503: return "GL_STACK_OVERFLOW";
		case 0x0504: return "GL_STACK_UNDERFLOW";
		case 0x0505: return "GL_OUT_OF_MEMORY";
		case 0x0506: return "GL_INVALID_FRAMEBUFFER_OPERATION";
		case 0x0507: return "GL_CONTEXT_LOST";
		case 0x8031: return "GL_TABLE_TOO_LARGE";
		}
		return "Unknown error";
	}

}

#ifndef BLT_DIST
#define GL_CALL(call) call; { int result = glGetError(); BLT_ASSERT(result == GL_NO_ERROR, "OPENGL ERROR : {0} ({1})", ::Bolt::ConvertError(result), result); }
#else
#define GL_CALL(call) call;
#endif