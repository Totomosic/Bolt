#pragma once
#include "Types.h"
#include "Timer.h"

namespace Bolt
{

	class BLT_API Time
	{
	private:
		static double s_TimeScale;

		static std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::duration<double>> s_StartTime;
		static double s_CurrentTime;
		static double s_ElapsedTime;
		static double s_RealCurrentTime;

		static std::vector<Timer> s_Timers;

	public:
		Time() = delete;

	public:
		static double ElapsedTime();
		static double DeltaTime();
		static double CurrentTime();
		static double& TimeScale();

		static double FramesPerSecond();

		static void Reset();
		static Timer* GetTimer(id_t id);
		static id_t CreateTimer(double time, RepeatMode repeat = RepeatMode::Recurring, bool start = true);
		static id_t CreateTimer(double time, Timer::TimerFunc callback, RepeatMode repeat = RepeatMode::Recurring, bool start = true);

		friend class Engine;

	private:
		static void Update();

	};

}