#include "bltpch.h"
#include "ThreadPool.h"

namespace Bolt
{

	ThreadPool::ThreadPool(int size)
		: m_Workers(), m_Jobs(), m_Running(true), m_ThreadStatusMutex(), m_ThreadJobMutex(), m_Size(size), m_Cv()
	{
	}

	ThreadPool::~ThreadPool()
	{
		Shutdown();
	}

	bool ThreadPool::IsRunning() const
	{
		return m_Running;
	}

	bool ThreadPool::IsEmpty() const
	{
		return m_Jobs.empty();
	}

	void ThreadPool::Enqueue(const ICommand& command)
	{
		{
			std::scoped_lock<std::mutex> lock(m_ThreadJobMutex);
			m_Jobs.push(command);
		}
		m_Cv.notify_one();
	}

	void ThreadPool::Start()
	{
		m_Running = true;
		for (int i = 0; i < m_Size; i++)
		{
			m_Workers.push_back(std::make_unique<Worker>(*this));
		}
	}

	void ThreadPool::Shutdown()
	{
		{
			std::scoped_lock<std::mutex> lock(m_ThreadStatusMutex);
			m_Running = false;
		}
		m_Cv.notify_all();
		for (std::unique_ptr<Worker>& worker : m_Workers)
		{
			worker->Join();
		}
		m_Workers.clear();
	}

	void ThreadPool::Resize(int size)
	{
		Shutdown();
		m_Size = size;
		Start();
	}

	std::queue<ThreadPool::ICommand>& Bolt::ThreadPool::GetQueue()
	{
		return m_Jobs;
	}

	std::condition_variable& ThreadPool::GetCondition()
	{
		return m_Cv;
	}

	std::mutex& ThreadPool::GetThreadMutex()
	{
		return m_ThreadJobMutex;
	}

}
