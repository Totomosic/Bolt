#include "bltpch.h"
#include "Profiling.h"

namespace Bolt
{

	Instrumentor& Instrumentor::Get()
	{
		static Instrumentor instance;
		return instance;
	}

	Instrumentor::Instrumentor()
		: m_Mutex(), m_CurrentSession(nullptr), m_ProfileCount(0)
	{
	}

	void Instrumentor::BeginSession(const blt::string& name, const blt::string& filepath)
	{
		std::scoped_lock<std::mutex> lock(m_Mutex);
		m_OutputStream.open(filepath.cpp_str());
		WriteHeader();
		m_CurrentSession = std::make_unique<InstrumentationSession>(InstrumentationSession{ name });
	}

	void Instrumentor::EndSession()
	{
		std::scoped_lock<std::mutex> lock(m_Mutex);
		WriteFooter();
		m_OutputStream.close();
		m_CurrentSession = nullptr;
		m_ProfileCount = 0;
	}

	void Instrumentor::WriteProfile(const ProfileResult& result)
	{
		std::scoped_lock<std::mutex> lock(m_Mutex);
		if (m_ProfileCount++ > 0)
		{
			m_OutputStream << ",";
		}

		blt::string name = result.Name;
		std::replace(name.begin(), name.end(), '"', '\'');

		m_OutputStream << "{";
		m_OutputStream << "\"cat\":\"function\",";
		m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
		m_OutputStream << "\"name\":\"" << name << "\",";
		m_OutputStream << "\"ph\":\"X\",";
		m_OutputStream << "\"pid\":0,";
		m_OutputStream << "\"tid\":" << result.ThreadId << ",";
		m_OutputStream << "\"ts\":" << result.Start;
		m_OutputStream << "}";

		m_OutputStream.flush();
	}

	void Instrumentor::WriteHeader()
	{
		m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
		m_OutputStream.flush();
	}

	void Instrumentor::WriteFooter()
	{
		m_OutputStream << "]}";
		m_OutputStream.flush();
	}

	InstrumentationTimer::InstrumentationTimer(const char* name)
		: m_Name(name), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	InstrumentationTimer::~InstrumentationTimer()
	{
		if (!m_Stopped)
		{
			Stop();
		}
	}

	void InstrumentationTimer::Stop()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> endTimepoint = std::chrono::high_resolution_clock::now();
		int64_t start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		int64_t end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
		uint32_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
		Instrumentor::Get().WriteProfile({ m_Name, start, end, threadId });
		m_Stopped = true;
	}

}
