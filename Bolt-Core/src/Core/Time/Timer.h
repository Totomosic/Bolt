#pragma once
#include "Types.h"

namespace Bolt
{

	BLT_API enum class RepeatMode
	{
		Recurring,
		Once
	};

	class BLT_API Timer
	{
	public:
		typedef std::function<void()> TimerFunc;

	private:
		double m_Time;
		double m_CurrentTime;
		TimerFunc m_Callback;
		bool m_UseCallback;
		bool m_IsRunning;
		RepeatMode m_Repeat;

	public:
		Timer(double time, RepeatMode repeat = RepeatMode::Recurring, bool start = true);
		Timer(double time, TimerFunc callback, RepeatMode repeat = RepeatMode::Recurring, bool start = true);

		double TotalTime() const;
		double& TotalTime();
		double CurrentTime() const;
		
		void Start();
		void Stop();
		void Reset();
		void SetTotalTime(double time);

		friend class Time;

	private:
		bool Update(double elapsedTime);

	};

}