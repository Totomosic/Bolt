#include "Types.h"
#include "Timeline.h"

namespace Bolt
{

	Timeline::Timeline(double timeScale)
		: m_RealCurrentTime(0.0), m_CurrentTime(0.0), m_ElapsedTime(0.0), m_TimeScale(timeScale), m_IsPaused(false)
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

	void Timeline::Update(double elapsedRealSeconds)
	{
		m_RealCurrentTime += elapsedRealSeconds;
		if (!IsPaused())
		{
			m_CurrentTime += elapsedRealSeconds * m_TimeScale;
			m_ElapsedTime = elapsedRealSeconds * m_TimeScale;
		}
		else
		{
			m_ElapsedTime = 0.0;
		}
	}

}