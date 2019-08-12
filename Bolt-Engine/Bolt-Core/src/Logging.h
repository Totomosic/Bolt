#pragma once
#include <Windows.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>

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
		static std::shared_ptr<spdlog::logger> s_LuaLogger;

	public:
		static void Initialize();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetLuaLogger() { return s_LuaLogger; }

	};

#ifdef BLT_DIST 
	#define BLT_CORE_TRACE(...)
	#define BLT_CORE_INFO(...)
	#define BLT_CORE_WARN(...)
	#define BLT_CORE_ERROR(...)
	#define BLT_CORE_FATAL(...)

	#define BLT_TRACE(...)
	#define BLT_INFO(...)
	#define BLT_WARN(...)
	#define BLT_ERROR(...)
	#define BLT_FATAL(...)

	#define BLT_LUA_TRACE(...)
	#define BLT_LUA_INFO(...)
	#define BLT_LUA_WARN(...)
	#define BLT_LUA_ERROR(...)
	#define BLT_LUA_FATAL(...)

	#define BLT_ASSERT(arg, ...)
#else
	#define BLT_CORE_TRACE(...) ::Bolt::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define BLT_CORE_INFO(...)  ::Bolt::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define BLT_CORE_WARN(...)  ::Bolt::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define BLT_CORE_ERROR(...) ::Bolt::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define BLT_CORE_FATAL(...) ::Bolt::Log::GetCoreLogger()->critical(__VA_ARGS__)

	#define BLT_TRACE(...) ::Bolt::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define BLT_INFO(...) ::Bolt::Log::GetClientLogger()->info(__VA_ARGS__)
	#define BLT_WARN(...) ::Bolt::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define BLT_ERROR(...) ::Bolt::Log::GetClientLogger()->error(__VA_ARGS__)
	#define BLT_FATAL(...) ::Bolt::Log::GetClientLogger()->critical(__VA_ARGS__)

	#define BLT_LUA_TRACE(...) ::Bolt::Log::GetLuaLogger()->trace(__VA_ARGS__)
	#define BLT_LUA_INFO(...) ::Bolt::Log::GetLuaLogger()->info(__VA_ARGS__)
	#define BLT_LUA_WARN(...) ::Bolt::Log::GetLuaLogger()->warn(__VA_ARGS__)
	#define BLT_LUA_ERROR(...) ::Bolt::Log::GetLuaLogger()->error(__VA_ARGS__)
	#define BLT_LUA_FATAL(...) ::Bolt::Log::GetLuaLogger()->critical(__VA_ARGS__)

	#define BLT_ASSERT(arg, ...) { if (!(arg)) { BLT_CORE_FATAL(__VA_ARGS__); __debugbreak(); } }
#endif

}