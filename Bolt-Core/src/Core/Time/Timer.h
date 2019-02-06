#pragma once
#include "TimerCallback.h"

namespace Bolt
{

	class BLT_API Timer
	{
	private:
		double m_Time;
		double m_CurrentTime;
		std::unique_ptr<TimerCallbackContainer> m_Callback;
		bool m_IsRunning;

	public:
		Timer(double time, std::unique_ptr<TimerCallbackContainer>&& callback, bool start = true);
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