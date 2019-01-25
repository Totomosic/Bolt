#include "Types.h"
#include "Timeline.h"

namespace Bolt
{

	Timeline::Timeline(double timeScale)
		: m_RealCurrentTime(0.0), m_CurrentTime(0.0), m_ElapsedTime(0.0), m_TimeScale(timeScale), m_IsPaused(false), m_TimersMutex(), m_Timers()
	{
	
	}

	double Timeline::CurrentRealTime() const
	{
		return m_RealCurrentTime;
	}

	double Timeline::CurrentTime() const
	{
		return m_CurrentTime;
	}

	double Timeline::ElapsedTime() const
	{
		return m_ElapsedTime;
	}

	double Timeline::DeltaTime() const
	{
		return ElapsedTime();
	}

	double Timeline::TimeScale() const
	{
		return m_TimeScale;
	}

	double& Timeline::TimeScale()
	{
		return m_TimeScale;
	}

	bool Timeline::IsPaused() const
	{
		return m_IsPaused;
	}

	void Timeline::SetPaused(bool paused)
	{
		m_IsPaused = paused;
	}

	void Timeline::Resume()
	{
		SetPaused(false);
	}

	void Timeline::Pause()
	{
		SetPaused(true);
	}

	void Timeline::Reset()
	{
		m_RealCurrentTime = 0.0;
		m_CurrentTime = 0.0;
		m_ElapsedTime = 0.0;
	}

	Timer& Timeline::AddTimer(double time, const Timer::TimerFunc& callback)
	{
		return AddTemporaryTimer(time, 0, callback);
	}

	Timer& Timeline::AddTemporaryTimer(double time, int invokeCount, const Timer::TimerFunc& callback)
	{
		std::scoped_lock<std::mutex> lock(m_TimersMutex);
		std::unique_ptr<Timer> timer = std::make_unique<Timer>(time, callback, true);
		Timer& result = *timer;
		m_Timers.AddTimerInfo({ std::move(timer), invokeCount });
		return result;
	}

	Timer& Timeline::AddTemporaryTimerByTime(double time, double timeToDelete, const Timer::TimerFunc& callback)
	{
		return AddTemporaryTimer(time, (int)(timeToDelete / time) + 1, callback);
	}

	Timer& Timeline::AddFunction(double time, const Timer::TimerFunc& callback)
	{
		return AddTemporaryTimer(time, 1, callback);
	}

	void Timeline::RemoveTimer(Timer* timer)
	{
		std::scoped_lock<std::mutex> lock(m_TimersMutex);
		m_Timers.RemoveTimer(timer);
	}

	void Timeline::Update(double elapsedRealSeconds)
	{
		m_RealCurrentTime += elapsedRealSeconds;
		if (!IsPaused())
		{
			m_CurrentTime += elapsedRealSeconds * m_TimeScale;
			m_ElapsedTime = elapsedRealSeconds * m_TimeScale;
			std::vector<TimerInfo> timers;
			{
				std::scoped_lock<std::mutex> lock(m_TimersMutex);
				timers = std::move(m_Timers.Queue());
				m_Timers.SwapQueues();
			}
			for (int i = timers.size() - 1; i >= 0; i--)
			{
				TimerInfo& timer = timers[i];
				if (timer.Timer->Update(m_ElapsedTime))
				{
					if (timer.InvokesLeft > 0)
					{
						timer.InvokesLeft -= 1;
						if (timer.InvokesLeft <= 0)
						{
							timers.erase(timers.begin() + i);
						}
					}
				}
			}			
			{
				std::scoped_lock<std::mutex> lock(m_TimersMutex);
				for (TimerInfo& info : timers)
				{
					m_Timers.AddTimerInfo(std::move(info));
				}
			}
		}
		else
		{
			m_ElapsedTime = 0.0;
		}
	}

}