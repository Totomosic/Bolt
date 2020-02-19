#pragma once
#include <chrono>
#include "Logging.h"

namespace Bolt
{

	class ScopedDebugTimer
	{
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
		std::string m_Function;

	public:
		ScopedDebugTimer(const std::string& functionName);
		~ScopedDebugTimer();
	};

}

#define BLT_TIME_SCOPE ::Bolt::ScopedDebugTimer _bltProfileTimer(__FUNCSIG__);