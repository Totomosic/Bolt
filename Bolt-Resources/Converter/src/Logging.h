#pragma once
#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>

#ifdef BLT_PLATFORM_WINDOWS
#define BLT_API
#else
#error Only Supports Windows
#endif

namespace Bolt
{

	class BLT_API Log
	{
	private:
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	public:
		static void Initialize();

		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	};

#ifdef BLT_DIST 

#define BLT_TRACE(...)
#define BLT_INFO(...)
#define BLT_WARN(...)
#define BLT_ERROR(...)
#define BLT_FATAL(...)

#define BLT_ASSERT(arg, ...)

#define BLT_DEBUG_ONLY(x)
#else
#define BLT_TRACE(...) ::Bolt::Log::GetClientLogger()->trace(__VA_ARGS__)
#define BLT_INFO(...) ::Bolt::Log::GetClientLogger()->info(__VA_ARGS__)
#define BLT_WARN(...) ::Bolt::Log::GetClientLogger()->warn(__VA_ARGS__)
#define BLT_ERROR(...) ::Bolt::Log::GetClientLogger()->error(__VA_ARGS__)
#define BLT_FATAL(...) ::Bolt::Log::GetClientLogger()->critical(__VA_ARGS__)

#define BLT_ASSERT(arg, ...) { if (!(arg)) { BLT_CORE_FATAL(__VA_ARGS__); __debugbreak(); } }

#define BLT_DEBUG_ONLY(x) x
#endif

}