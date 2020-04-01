#include "bltpch.h"
#include "Worker.h"
#include "ThreadPool.h"

namespace Bolt
{

	Worker::Worker(ThreadPool& pool)
		: m_Status(false), m_Thread(), m_Running(false), m_Pool(pool)
	{
		m_Thread = std::make_unique<std::thread>(&Worker::ThreadFunction, this);
	}

	void Worker::Join()
	{
		m_Thread->join();
	}

	void Worker::ThreadFunction()
	{
		while (m_Pool.IsRunning())
		{
			ThreadPool::ICommand cmd;
			{
				std::unique_lock<std::mutex> lock(m_Pool.GetThreadMutex());
				m_Pool.GetCondition().wait(lock, [this]()
					{
						return !m_Pool.IsRunning() || !m_Pool.IsEmpty();
					});
				if (!m_Pool.IsRunning() && m_Pool.IsEmpty())
					return;
				cmd = std::move(m_Pool.GetQueue().front());
				m_Pool.GetQueue().pop();
			}
			cmd();
		}
	}

}
