#pragma once
#include "Task.h"
#include "Core/Events/EventBus.h"
#include "SharedFunction.h"

namespace Bolt
{

	class BLT_API TaskManager
	{
	public:
		static TaskManager& Get();

	private:
		EventBus& m_Bus;
		ThreadPool m_Pool;
		ScopedEventListener m_Listener;

	public:
		TaskManager(EventBus& bus);

		EventBus& Bus() const;
		ThreadPool& GetThreadPool();

		template<typename DelegateT, typename TResult = typename std::result_of<DelegateT()>::type>
		Task<TResult> Run(DelegateT func)
		{
			Task<TResult> task(TaskLauncher{ m_Pool }, Bus(), std::move(func));
			return task;
		}

		template<typename DelegateT>
		void RunOnMainThread(DelegateT func)
		{
			m_Bus.Emit(TaskBase::TaskEventId, TaskCompleted<int>(0, [func{ std::move(func) }](int ignore) mutable
			{
				func();
			}));
		}

	};

}