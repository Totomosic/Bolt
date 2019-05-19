#include "bltpch.h"

#include "Time.h"
#include "Engine/Engine.h"

namespace Bolt
{

	Time& Time::Get()
	{
		return Engine::Instance().CurrentContext().GetRenderContext().GetTime();
	}

	Time::Time()
		: m_StartTime(std::chrono::high_resolution_clock::now()), m_PrevTime(0.0), m_RenderingTimeline(1.0)
	{
	
	}

	Timeline& Time::RenderingTimeline()
	{
		return m_RenderingTimeline;
	}

	double Time::FramesPerSecond()
	{
		return 1.0 / m_RenderingTimeline.ElapsedTime();
	}

	void Time::Reset()
	{
		glfwSetTime(0.0);
		m_StartTime = std::chrono::high_resolution_clock::now();
		m_RenderingTimeline.Reset();
		m_PrevTime = 0.0;
	}

	void Time::Update()
	{
		double current = (std::chrono::high_resolution_clock::now() - m_StartTime).count();
		double currentSeconds = current / 1e9;
		m_RenderingTimeline.Update(currentSeconds - m_PrevTime);
		m_PrevTime = currentSeconds;
	}

}