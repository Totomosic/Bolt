#pragma once
#include "Timer.h"
#include "Timeline.h"
#include "TimeDelta.h"

namespace Bolt
{

	class BLT_API Time
	{
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::duration<double>> m_StartTime;
		double m_PrevTime;
		Timeline m_RenderingTimeline;

	public:
		static Time& Get();

	public:
		Time();

		Timeline& RenderingTimeline();

		double FramesPerSecond();

		void Reset();

		friend class Engine;
		friend class Application;

	private:
		void Update();

	};

}