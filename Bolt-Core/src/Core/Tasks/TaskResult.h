#pragma once
#include "bltpch.h"

namespace Bolt
{

	template<typename T>
	class BLT_API TaskState
	{
	public:
		std::promise<T> Promise;
		std::future<T> Future;

	public:
		TaskState()
			: Promise(), Future(Promise.get_future())
		{
		}
	};

	template<typename T>
	class BLT_API TaskResult
	{
	public:
		std::shared_ptr<TaskState<T>> m_State;

	public:
		TaskResult()
			: m_State(std::make_shared<TaskState<T>>())
		{
			
		}

		T Get()
		{
			return m_State->Future.get();
		}

		void Wait()
		{
			m_State->Future.wait();
		}

		std::future_status WaitFor(double seconds)
		{
			return m_State->Future.wait_for(std::chrono::nanoseconds((size_t)(seconds * 1e9)));
		}

	};

	template<typename TFuncResult, typename TResult = TFuncResult>
	TaskResult<TResult> LaunchAsync(std::function<TFuncResult()> func)
	{
		TaskResult<TResult> result;
		std::thread t([statePtr{ result.m_State }, func{ std::move(func) }]()
		{
			statePtr->Promise.set_value(func());
		});
		t.detach();
		return result;
	}

}