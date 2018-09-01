#pragma once
#include <Windows.h>

namespace Bolt
{

#define BLT_LOG_LEVEL_INFO 0
#define BLT_LOG_LEVEL_WARN 1
#define BLT_LOG_LEVEL_ERROR 2
#define BLT_LOG_LEVEL_FATAL 3

	inline void SetupConsole(int level)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		switch (level)
		{
		case (BLT_LOG_LEVEL_FATAL):
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY);
			break;
		case (BLT_LOG_LEVEL_ERROR):
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case (BLT_LOG_LEVEL_WARN):
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			break;
		case (BLT_LOG_LEVEL_INFO):
			SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			break;
		}
	}

	template<typename T>
	inline void PrintMessage(int level, const T& msg)
	{
		SetupConsole(level);
		std::cout << "Bolt:   " << msg << std::endl;
	}

#ifdef BLT_DEBUG
	#define BLT_INFO(msg) Bolt::PrintMessage(BLT_LOG_LEVEL_INFO, msg);
	#define BLT_WARN(msg) Bolt::PrintMessage(BLT_LOG_LEVEL_WARN, msg);
	#define BLT_ERROR(msg) Bolt::PrintMessage(BLT_LOG_LEVEL_ERROR, msg);
	#define BLT_FATAL(msg) Bolt::PrintMessage(BLT_LOG_LEVEL_FATAL, msg);

	#define BLT_ASSERT(arg, msg) { if (!(arg)) { BLT_FATAL(msg); __debugbreak(); } }
#else
	#define BLT_INFO(msg)
	#define BLT_WARN(msg)
	#define BLT_ERROR(msg)
	#define BLT_FATAL(msg)

	#define BLT_ASSERT(arg, msg)
#endif

}