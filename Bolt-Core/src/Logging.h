#pragma once
#include <Windows.h>
#include <spdlog\spdlog.h>
#include <spdlog\sinks\stdout_color_sinks.h>
#include <spdlog\fmt\ostr.h>

#ifdef BLT_PLATFORM_WINDOWS
#ifdef BLT_BUILD_STATIC
#define BLT_API
#elif BLT_BUILD_DLL
#define BLT_API __declspec(dllexport)
#else
#define BLT_API __declspec(dllimport)
#endif
#else
#error Only Supports Windows
#endif

namespace Bolt
{

	class BLT_API Log
	{
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	public:
		static void Initialize();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	};

#ifdef BLT_DEBUG
	#define BLT_CORE_TRACE(...) ::Bolt::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define BLT_CORE_INFO(...)  ::Bolt::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define BLT_CORE_WARN(...)  ::Bolt::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define BLT_CORE_ERROR(...) ::Bolt::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define BLT_CORE_FATAL(...) ::Bolt::Log::GetCoreLogger()->critical(__VA_ARGS__)

	#define BLT_ASSERT(arg, ...) { if (!(arg)) { BLT_CORE_FATAL(__VA_ARGS__); __debugbreak(); } }
#else
	#define BLT_CORE_INFO(...)
	#define BLT_CORE_WARN(...)
	#define BLT_CORE_ERROR(...)
	#define BLT_CORE_FATAL(...)

	#define BLT_ASSERT(arg, ...)
#endif

}