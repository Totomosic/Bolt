#include "Types.h"
#include "TimerSet.h"

namespace Bolt
{

	TimerSet::TimerSet()
		: m_Queue(), m_BackQueue()
	{
	
	}

	size_t TimerSet::Length() const
	{
		return m_Queue.size();
	}

	const std::vector<TimerInfo>& TimerSet::Queue() const
	{
		return m_Queue;
	}

	std::vector<TimerInfo>& TimerSet::Queue()
	{
		return m_Queue;
	}

	void TimerSet::AddTimerInfo(TimerInfo&& info)
	{
		m_Queue.push_back(std::move(info));
	}

	void TimerSet::RemoveTimer(Timer* timer)
	{
		for (int i = 0; i < Length(); i++)
		{
			if (m_Queue[i].Timer.get() == timer)
			{
				m_Queue.erase(m_Queue.begin() + 1);
				break;
			}
		}
	}

	void TimerSet::SwapQueues()
	{
		m_Queue = std::move(m_BackQueue);
		m_BackQueue = std::vector<TimerInfo>();
	}

}