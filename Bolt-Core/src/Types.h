#pragma once
#include "FreeType-GL\include\freetype-gl.h"
#include "glad\glad.h"
#include "GLFW\glfw3.h"

#include "Core\BoltTL\__BoltTL__.h"
#include "BoltDefines.h"

#include <Python.h>
#include <algorithm>
#include <bitset>
#include <chrono>
#include <cstdlib>
#include <deque>
#include <direct.h>
#include <fstream>
#include <functional>
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

	typedef uint8_t byte;
	typedef size_t uint;
	typedef int64_t int64;
	typedef uint64_t uint64;

	typedef uint id_t;

	typedef float real;

}