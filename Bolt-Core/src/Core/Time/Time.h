#pragma once
#include "Timer.h"
#include "Timeline.h"

namespace Bolt
{

	class BLT_API Time
	{
	private:
		static std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::duration<double>> s_StartTime;
		static Timeline s_RenderingTimeline;

		static std::vector<Timer> s_Timers;

	public:
		Time() = delete;

	public:
		static Timeline& RenderingTimeline();

		static double FramesPerSecond();

		static void Reset();
		static Timer* GetTimer(id_t id);
		static id_t CreateTimer(double time, RepeatMode repeat = RepeatMode::Recurring, bool start = true);
		static id_t CreateTimer(double time, Timer::TimerFunc callback, RepeatMode repeat = RepeatMode::Recurring, bool start = true);

		friend class Engine;
		friend class Application;

	private:
		static void Update();

	};

}