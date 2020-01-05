#pragma once
#include "TaskResult.h"
#include "TaskEvents.h"
#include "Core/Events/EventManager.h"

namespace Bolt
{

	BLT_API enum class TaskStatus
	{
		Ready,
		Timeout
	};

	template<class TResult>
	class BLT_API Task
	{
	private:
		TaskResult<TResult> m_Result;

	private:
		template<typename DelegateT, typename TaskResultT>
		Task(DelegateT func, Task<TaskResultT>& taskResult)
		{
			m_Result = LaunchAsync<TResult>(std::function<TResult()>([taskResult = std::move(taskResult.m_Result), del = std::move(func)]() mutable -> TResult
			{
				TaskResult<TaskResultT>& tResult = (TaskResult<TaskResultT>&)taskResult;
				return del(tResult.Get());
			}));
		}

		template<typename DelegateT>
		Task(DelegateT func, Task<void>& taskResult)
		{
			m_Result = LaunchAsync<TResult>(std::function<TResult()>([taskResult = std::move(taskResult.m_Result), del = std::move(func)]() mutable -> TResult
			{
				TaskResult<int>& tResult = (TaskResult<int>&)taskResult;
				tResult.Get();
				return del();
			}));
		}

	public:
		template<typename DelegateT>
		Task(DelegateT func)
		{
			m_Result = LaunchAsync<TResult>(std::function<TResult()>([del = std::move(func)]() mutable -> TResult
			{
				return del();
			}));
		}

		TResult Result()
		{
			return m_Result.Get();
		}

		void Wait()
		{
			m_Result.Wait();
		}

		TaskStatus WaitFor(double seconds)
		{
			std::future_status status = m_Result.WaitFor(seconds);
			if (status == std::future_status::ready)
			{
				return TaskStatus::Ready;
			}
			return TaskStatus::Timeout;
		}

		template<typename DelegateT, typename TNewResult = std::result_of<DelegateT(TResult)>::type>
		Task<TNewResult> ContinueWith(DelegateT func)
		{
			return Task<TNewResult>(std::move(func), *this);
		}

		template<typename DelegateT>
		void ContinueWithOnMainThread(DelegateT func)
		{
			ContinueWith([func{ std::move(func) }](TResult value) mutable
			{
				EventManager::Get().Bus().Emit(Events::Internal.AsyncTaskCompleted, TaskCompleted<TResult>(std::move(value), std::move(func)));
			});
		}

		template<typename> friend class Task;

	};

	template<>
	class BLT_API Task<void>
	{
	private:
		TaskResult<int> m_Result;

	private:
		template<typename DelegateT, typename TaskResultT>
		Task(DelegateT func, Task<TaskResultT>& taskResult)
		{
			m_Result = LaunchAsync<int>(std::function<int()>([taskResult = std::move(taskResult.m_Result), del = std::move(func)]() mutable -> int
			{
				TaskResult<TaskResultT>& tResult = (TaskResult<TaskResultT>&)taskResult;				
				del(tResult.Get());
				return 0;
			}));
		}

		template<typename DelegateT>
		Task(DelegateT func, Task<void>& taskResult)
		{
			m_Result = LaunchAsync<int>(std::function<int()>([taskResult = std::move(taskResult.m_Result), del = std::move(func)]() mutable -> int
			{
				TaskResult<int>& tResult = (TaskResult<int>&)taskResult;
				tResult.Get();
				del();
				return 0;
			}));
		}

	public:
		template<typename DelegateT>
		Task(DelegateT func)
		{
			m_Result = LaunchAsync<int>(std::function<int()>([del = std::move(func)]() mutable -> int
			{
				del();
				return 0;
			}));
		}

		void Wait()
		{
			m_Result.Get();
		}

		template<typename DelegateT, typename TNewResult = std::result_of<DelegateT()>::type>
		Task<TNewResult> ContinueWith(DelegateT func)
		{
			return Task<TNewResult>(std::move(func), *this);
		}

		template<typename DelegateT>
		void ContinueWithOnMainThread(DelegateT func)
		{
			ContinueWith([func{ std::move(func) }]() mutable
			{
				EventManager::Get().Bus().Emit(Events::Internal.AsyncTaskCompleted, TaskCompleted<int>(0, [func{ std::move(func) }](int ignore) mutable
				{
					func();
				}));
			});
		}

		template<typename> friend class Task;
	};

}