#pragma once
#include "Types.h"

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

		void Update(double elapsedRealSeconds);

	};

}