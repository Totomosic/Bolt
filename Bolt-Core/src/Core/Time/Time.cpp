#include "Time.h"

namespace Bolt
{
	
	double Time::s_TimeScale = 1.0;
	std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::duration<double>> Time::s_StartTime = std::chrono::high_resolution_clock::now();
	double Time::s_CurrentTime = 0.0;
	double Time::s_ElapsedTime = 0.0;
	double Time::s_RealCurrentTime = 0.0;

	std::vector<Timer> Time::s_Timers = std::vector<Timer>();

	double Time::DeltaTime()
	{
		return s_ElapsedTime;
	}

	double Time::ElapsedTime()
	{
		return DeltaTime();
	}

	double Time::CurrentTime()
	{
		return (((std::chrono::high_resolution_clock::now() - s_StartTime).count() - s_RealCurrentTime) * s_TimeScale + s_CurrentTime) / 1e9;
	}

	double& Time::TimeScale()
	{
		return s_TimeScale;
	}

	double Time::FramesPerSecond()
	{
		return 1.0 / ElapsedTime();
	}

	void Time::Reset()
	{
		glfwSetTime(0.0);
		s_ElapsedTime = 0.0;
		s_StartTime = std::chrono::high_resolution_clock::now();
		s_CurrentTime = 0.0;
		s_RealCurrentTime = 0.0;
	}

	Timer* Time::CreateTimer(double time, RepeatMode repeat, bool start)
	{
		size_t index = s_Timers.size();
		s_Timers.emplace_back(time, repeat, start);
		return &s_Timers.at(index);
	}

	Timer* Time::CreateTimer(double time, Timer::TimerFunc callback, RepeatMode repeat, bool start)
	{
		size_t index = s_Timers.size();
		s_Timers.emplace_back(time, callback, repeat, start);
		return &s_Timers.at(index);
	}

	void Time::Update()
	{
		double prev = s_CurrentTime;
		s_CurrentTime = (std::chrono::high_resolution_clock::now() - s_StartTime).count() * s_TimeScale;
		s_RealCurrentTime = (std::chrono::high_resolution_clock::now() - s_StartTime).count();
		s_ElapsedTime = (s_CurrentTime - prev) / 1e9;

		for (int i = s_Timers.size() - 1; i >= 0; i--)
		{
			Timer& t = s_Timers[i];
			if (t.Update(s_ElapsedTime))
			{
				s_Timers.erase(s_Timers.begin() + i);
			}
		}
	}

}