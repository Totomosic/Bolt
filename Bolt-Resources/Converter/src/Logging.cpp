#include "Logging.h"

namespace Bolt
{

	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Initialize()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_ClientLogger = spdlog::stdout_color_mt("[BOLT-RESOURCES]");
		s_ClientLogger->set_level(spdlog::level::level_enum::trace);
	}

}