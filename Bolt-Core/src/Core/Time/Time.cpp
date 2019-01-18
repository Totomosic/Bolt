#include "Types.h"

#include "Time.h"

namespace Bolt
{
	
	std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::duration<double>> Time::s_StartTime = std::chrono::high_resolution_clock::now();
	Timeline Time::s_RenderingTimeline = Timeline(1.0);

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

	void Time::Update()
	{
		double prev = s_RenderingTimeline.CurrentRealTime();
		double current = (std::chrono::high_resolution_clock::now() - s_StartTime).count();
		s_RenderingTimeline.Update(current / 1e9 - prev);
	}

}