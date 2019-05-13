#pragma once
#include "Timer.h"

namespace Bolt
{

	struct BLT_API TimerInfo
	{
	public:
		std::unique_ptr<Timer> Timer;
		int InvokesLeft;
	};

	class BLT_API TimerSet
	{
	private:
		std::vector<TimerInfo> m_Queue;
		std::vector<TimerInfo> m_BackQueue;

	public:
		TimerSet();

		uint32_t Length() const;
		const std::vector<TimerInfo>& Queue() const;
		std::vector<TimerInfo>& Queue();

		void AddTimerInfo(TimerInfo&& info);
		void RemoveTimer(Timer* timer);
		void SwapQueues();

	};

}