#pragma once
#include "bltpch.h"

namespace Bolt
{

	struct BLT_API ProfileResult
	{
		blt::string Name;
		int64_t Start;
		int64_t End;
		uint32_t ThreadId;
	};

	struct BLT_API InstrumentationSession
	{
		blt::string Name;
	};

	class BLT_API Instrumentor
	{
	private:
		std::mutex m_Mutex;
		std::unique_ptr<InstrumentationSession> m_CurrentSession;
		std::ofstream m_OutputStream;
		int m_ProfileCount;

	public:
		static Instrumentor& Get();

	public:
		Instrumentor();

		void BeginSession(const blt::string& name, const blt::string& filepath = "results.json");
		void EndSession();

		void WriteProfile(const ProfileResult& result);
		void WriteHeader();
		void WriteFooter();
	};

	class BLT_API InstrumentationTimer
	{
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
		bool m_Stopped;

	public:
		InstrumentationTimer(const char* name);
		~InstrumentationTimer();

		void Stop();
	};

#if BLT_PROFILE
	#define BLT_PROFILE_BEGIN_SESSION(name, filepath) ::Bolt::Instrumentor::Get().BeginSession(name, filepath)
	#define BLT_PROFILE_END_SESSION() ::Bolt::Instrumentor::Get().EndSession()
	#define BLT_PROFILE_SCOPE(name) ::Bolt::InstrumentationTimer _profileTimer##__LINE__(name)
	#define BLT_PROFILE_FUNCTION() BLT_PROFILE_SCOPE(__FUNCSIG__)
	#if BLT_PROFILE_RENDERER
		#define BLT_PROFILE_RENDERER_SCOPE(name) BLT_PROFILE_SCOPE(name)
		#define BLT_PROFILE_RENDERER_FUNCTION() BLT_PROFILE_FUNCTION()
	#else
		#define BLT_PROFILE_RENDERER_SCOPE(name)
		#define BLT_PROFILE_RENDERER_FUNCTION()
	#endif
#else
	#define BLT_PROFILE_BEGIN_SESSION(name, filepath)
	#define BLT_PROFILE_END_SESSION()
	#define BLT_PROFILE_SCOPE(name)
	#define BLT_PROFILE_FUNCTION()
	#define BLT_PROFILE_RENDERER_SCOPE(name)
	#define BLT_PROFILE_RENDERER_FUNCTION()
#endif

}