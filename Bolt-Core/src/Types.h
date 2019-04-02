#pragma once
#include "FreeType-GL\include\freetype-gl.h"
#include "glad\glad.h"
#include "GLFW\glfw3.h"
#include "imgui.h"

#include "Core\BoltTL\__BoltTL__.h"
#include "BoltDefines.h"

#include <algorithm>
#include <bitset>
#include <chrono>
#include <cstdlib>
#include <deque>
#include <direct.h>
#include <fstream>
#include <functional>
#include <future>
#include <ios>
#include <iostream>
#include <memory>
#include <mutex>
#include <numeric>
#include <ratio>
#include <sstream>
#include <stdio.h>
#include <string>
#include <thread>
#include <typeindex>
#include <typeinfo>
#include <type_traits>

#include <WinSock2.h>
#include <WS2tcpip.h>

#define SCK_VERSION 0x0202

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

namespace Bolt
{

	using byte = uint8_t;
	using uint = size_t;
	using int64 = int64_t;
	using uint64 = uint64_t;
	using id_t = uint;
	using real = float;

}