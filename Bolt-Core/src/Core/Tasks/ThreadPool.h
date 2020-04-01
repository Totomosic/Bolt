#pragma once
#include "Worker.h"

namespace Bolt
{

	class BLT_API ThreadPool
	{
	public:
		using ICommand = std::function<void()>;

	private:
		std::vector<std::unique_ptr<Worker>> m_Workers;
		std::queue<ICommand> m_Jobs;
		bool m_Running;
		std::mutex m_ThreadStatusMutex;
		std::mutex m_ThreadJobMutex;
		int m_Size;
		std::condition_variable m_Cv;
		
	public:
		ThreadPool(int size = std::thread::hardware_concurrency());
		~ThreadPool();

		bool IsRunning() const;
		bool IsEmpty() const;

		void Enqueue(const ICommand& command);
		void Start();
		void Shutdown();

		void Resize(int size);

		friend class Worker;

	private:
		std::queue<ICommand>& GetQueue();
		std::condition_variable& GetCondition();
		std::mutex& GetThreadMutex();
	};

}