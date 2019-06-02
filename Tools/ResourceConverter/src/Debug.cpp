#include "Debug.h"

namespace Bolt
{

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;

	void Log::Initialize()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("[BOLT-RESOURCES]");
		s_CoreLogger->set_level(spdlog::level::level_enum::trace);
	}

}