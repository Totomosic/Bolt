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

	public:
		Time() = delete;

	public:
		static Timeline& RenderingTimeline();

		static double FramesPerSecond();

		static void Reset();

		friend class Engine;
		friend class Application;

	private:
		static void Update();

	};

}