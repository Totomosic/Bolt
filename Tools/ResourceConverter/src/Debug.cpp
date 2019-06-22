#include "Debug.h"

namespace Bolt
{

	std::shared_ptr<spdlog::logger> Log::s_DebugLogger;
	std::shared_ptr<spdlog::logger> Log::s_ConsoleLogger;

	void Log::Initialize()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_DebugLogger = spdlog::stdout_color_mt("[DEBUG]");
		s_DebugLogger->set_level(spdlog::level::level_enum::trace);

		s_ConsoleLogger = spdlog::stdout_color_mt("[BOLT-RESOURCES]");
		s_ConsoleLogger->set_level(spdlog::level::level_enum::trace);
	}

}