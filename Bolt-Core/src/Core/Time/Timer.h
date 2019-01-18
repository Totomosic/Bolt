#pragma once
#include "Types.h"

namespace Bolt
{

	class BLT_API Timer
	{
	public:
		using TimerFunc = std::function<void()>;

	private:
		double m_Time;
		double m_CurrentTime;
		TimerFunc m_Callback;
		bool m_IsRunning;

	public:
		Timer(double time, TimerFunc callback, bool start = true);

		double TotalTime() const;
		double& TotalTime();
		double CurrentTime() const;
		
		void Start();
		void Stop();
		void Reset();
		void SetTotalTime(double time);

		friend class Timeline;

	private:
		bool Update(double elapsedTime);

	};

}