#pragma once
#include "bltpch.h"

namespace Bolt
{

	class ThreadPool;

	class BLT_API Worker
	{
	private:
		bool m_Status;
		std::unique_ptr<std::thread> m_Thread;
		bool m_Running;
		ThreadPool& m_Pool;

	public:
		Worker(ThreadPool& pool);

		void Join();

	private:
		void ThreadFunction();
	};

}