#pragma once
#include "Types.h"
#include "TimerSet.h"

namespace Bolt
{

	class BLT_API Timeline
	{
	private:
		// Current time disregarding pauses/timescale changes
		double m_RealCurrentTime;
		// Current time accounting for pauses/timescale changes
		double m_CurrentTime;
		// Elapsed time since last update
		double m_ElapsedTime;
		// Ratio between speed on time on this scale relative to real time
		double m_TimeScale;
		// Is Paused
		bool m_IsPaused;

		std::mutex m_TimersMutex;
		TimerSet m_Timers;

	public:
		Timeline(double timeScale = 1.0f);

		double CurrentRealTime() const;
		double CurrentTime() const;
		double ElapsedTime() const;
		double DeltaTime() const;
		double TimeScale() const;
		double& TimeScale();
		bool IsPaused() const;

		void SetPaused(bool paused);
		void Resume();
		void Pause();
		// Resets current and elapsed time, leaves time scale unchanged
		void Reset();

		Timer& AddTimer(double time, Timer::TimerFunc callback);
		Timer& AddFunction(double time, Timer::TimerFunc callback);
		Timer& AddTemporaryTimer(double time, int invokeCount, Timer::TimerFunc callback);
		Timer& AddTemporaryTimerByTime(double time, double timeToDelete, Timer::TimerFunc callback);
		void RemoveTimer(Timer* timer);

		void Update(double elapsedRealSeconds);

	};

}