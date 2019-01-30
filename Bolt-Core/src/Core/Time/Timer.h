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
		Timer(double time, Timer::TimerFunc callback, bool start = true);
		Timer(const Timer& other) = delete;
		Timer& operator=(const Timer& other) = delete;
		Timer(Timer&& other) = default;
		Timer& operator=(Timer&& other) = default;
		~Timer() = default;

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