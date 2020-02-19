#include "DebugTimer.h"

namespace Bolt
{

	ScopedDebugTimer::ScopedDebugTimer(const std::string& functionName)
		: m_StartTime(std::chrono::high_resolution_clock::now()), m_Function(functionName)
	{
	}

	ScopedDebugTimer::~ScopedDebugTimer()
	{
		size_t microseconds = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_StartTime).count();
		BLT_INFO("{0} took {1}us", m_Function, microseconds);
	}

}
