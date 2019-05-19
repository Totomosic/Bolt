#include "bltpch.h"
#include "Timer.h"

namespace Bolt
{

	Timer::Timer(double time, std::unique_ptr<TimerCallbackContainer>&& callback, bool start)
		: m_Time(time), m_CurrentTime(0.0), m_Callback(std::move(callback)), m_IsRunning(start)
	{
		
	}

	double Timer::TotalTime() const
	{
		return m_Time;
	}

	double& Timer::TotalTime()
	{
		return m_Time;
	}

	double Timer::CurrentTime() const
	{
		return m_CurrentTime;
	}

	void Timer::Start()
	{
		m_IsRunning = true;
	}

	void Timer::Stop()
	{
		m_IsRunning = false;
	}

	void Timer::Reset()
	{
		m_CurrentTime = 0.0;
	}

	void Timer::SetTotalTime(double time)
	{
		m_Time = time;
	}

	bool Timer::Update(double elapsedTime)
	{
		if (m_IsRunning)
		{
			m_CurrentTime += elapsedTime;
			if (m_CurrentTime >= m_Time)
			{
				(*m_Callback)();
				m_CurrentTime = 0.0;
				return true;
			}
		}
		return false;
	}

}