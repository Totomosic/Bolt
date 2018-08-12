#pragma once
#include "FreeType-GL\include\freetype-gl.h"
#include "GLEW\include\GL\glew.h"
#include "GLFW\include\GLFW\glfw3.h"

#include "Core\BoltTL\__BoltTL__.h"
#include "BoltDefines.h"

#include <algorithm>
#include <bitset>
#include <chrono>
#include <cstdlib>
#include <direct.h>
#include <fstream>
#include <functional>
#include <ios>
#include <iostream>
#include <memory>
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

#define BLT_API

namespace Bolt
{

	typedef uint8_t byte;
	typedef size_t uint;
	typedef int64_t int64;
	typedef uint64_t uint64;

	typedef uint id_t;

	typedef float real;

}