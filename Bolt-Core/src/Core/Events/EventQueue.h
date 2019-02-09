#pragma once
#include "Types.h"

namespace Bolt
{

	template<typename T>
	class BLT_API EventQueue
	{
	private:
		T* m_Queue;
		T* m_QueueBackBuffer;
		size_t m_Capacity;
		size_t m_QueueTail;

	public:
		EventQueue(size_t maxEvents)
			: m_Queue(new T[maxEvents]{}), m_QueueBackBuffer(new T[maxEvents]{}), m_Capacity(maxEvents), m_QueueTail(0)
		{
			
		}

		EventQueue(const EventQueue<T>& other) = delete;
		EventQueue<T>& operator=(const EventQueue<T>& other) = delete;
		EventQueue(EventQueue<T>&& other) = delete;
		EventQueue<T>& operator=(EventQueue<T>&& other) = delete;

		~EventQueue()
		{
			delete[] m_Queue;
			delete[] m_QueueBackBuffer;
		}

		inline size_t MaxEvents() const { return m_Capacity; }
		inline size_t EventCount() const { return m_QueueTail; }
		inline T* GetQueuePtr() const { return m_Queue; }

		T& AddEvent(T e)
		{
			size_t currentIndex = m_QueueTail;
			if (currentIndex == MaxEvents())
			{
				currentIndex--;
				m_QueueTail--;
				BLT_CORE_WARN("EventQueue already contained {} events. Overriding most recent event.", MaxEvents());
			}
			m_Queue[currentIndex] = std::move(e);
			m_QueueTail++;
			return m_Queue[currentIndex];
		}

		void SwapQueues()
		{
			m_QueueTail = 0;
			T* queue = m_Queue;
			m_Queue = m_QueueBackBuffer;
			m_QueueBackBuffer = queue;
		}

	};

}