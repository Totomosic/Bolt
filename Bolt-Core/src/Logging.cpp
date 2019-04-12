#include "Types.h"

namespace Bolt
{

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
	std::shared_ptr<spdlog::logger> Log::s_LuaLogger;

	void Log::Initialize()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("[BOLT]");
		s_CoreLogger->set_level(spdlog::level::level_enum::trace);
		s_ClientLogger = spdlog::stdout_color_mt("[APP]");
		s_ClientLogger->set_level(spdlog::level::level_enum::trace);
		s_LuaLogger = spdlog::stdout_color_mt("[LUA]");
		s_LuaLogger->set_level(spdlog::level::level_enum::trace);
	}

}