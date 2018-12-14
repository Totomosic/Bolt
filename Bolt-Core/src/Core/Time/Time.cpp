#include "Types.h"

#include "Time.h"

namespace Bolt
{
	
	std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::duration<double>> Time::s_StartTime = std::chrono::high_resolution_clock::now();
	Timeline Time::s_RenderingTimeline = Timeline(1.0);

	std::vector<Timer> Time::s_Timers = std::vector<Timer>();

	Timeline& Time::RenderingTimeline()
	{
		return s_RenderingTimeline;
	}

	double Time::FramesPerSecond()
	{
		return 1.0 / s_RenderingTimeline.ElapsedTime();
	}

	void Time::Reset()
	{
		glfwSetTime(0.0);
		s_StartTime = std::chrono::high_resolution_clock::now();
		s_RenderingTimeline.Reset();
	}

	Timer* Time::GetTimer(id_t id)
	{
		return &s_Timers.at(id);
	}

	id_t Time::CreateTimer(double time, RepeatMode repeat, bool start)
	{
		size_t index = s_Timers.size();
		s_Timers.emplace_back(time, repeat, start);
		return index;
	}

	id_t Time::CreateTimer(double time, Timer::TimerFunc callback, RepeatMode repeat, bool start)
	{
		size_t index = s_Timers.size();
		s_Timers.emplace_back(time, callback, repeat, start);
		return index;
	}

	void Time::Update()
	{
		double prev = s_RenderingTimeline.CurrentRealTime();
		double current = (std::chrono::high_resolution_clock::now() - s_StartTime).count();
		s_RenderingTimeline.Update(current / 1e9 - prev);

		for (int i = s_Timers.size() - 1; i >= 0; i--)
		{
			Timer& t = s_Timers[i];
			if (t.Update(s_RenderingTimeline.ElapsedTime()))
			{
				s_Timers.erase(s_Timers.begin() + i);
			}
		}
	}

}