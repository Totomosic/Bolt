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
		Task(DelegateT func, Task<TaskResultT>& taskResult);

		template<typename DelegateT>
		Task(DelegateT func, Task<void>& taskResult);

	public:
		template<typename DelegateT>
		Task(DelegateT func);

		TResult Result();
		void Wait();
		TaskStatus WaitFor(double seconds);

		template<typename DelegateT, typename TNewResult = typename std::result_of<DelegateT(TResult)>::type>
		Task<TNewResult> ContinueWith(DelegateT func);

		template<typename DelegateT>
		void ContinueWithOnMainThread(DelegateT func);

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

		template<typename DelegateT, typename TNewResult = typename std::result_of<DelegateT()>::type>
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

	// =================================================================================================================================================================
	// TASK<T> IMPLEMENTATION
	// =================================================================================================================================================================

	template<typename T>
	template<typename DelegateT, typename TaskResultT>
	Task<T>::Task(DelegateT func, Task<TaskResultT>& taskResult)
	{
		m_Result = LaunchAsync<T>(std::function<T()>([taskResult = std::move(taskResult.m_Result), del = std::move(func)]() mutable-> T
		{
			TaskResult<TaskResultT>& tResult = (TaskResult<TaskResultT>&)taskResult;
			return del(tResult.Get());
		}));
	}

	template<typename T>
	template<typename DelegateT>
	Task<T>::Task(DelegateT func, Task<void>& taskResult)
	{
		m_Result = LaunchAsync<T>(std::function<T()>([taskResult = std::move(taskResult.m_Result), del = std::move(func)]() mutable-> T
		{
			TaskResult<int>& tResult = (TaskResult<int>&)taskResult;
			tResult.Get();
			return del();
		}));
	}

	template<typename T>
	template<typename DelegateT>
	Task<T>::Task(DelegateT func)
	{
		m_Result = LaunchAsync<T>(std::function<T()>([del = std::move(func)]() mutable-> T
		{
			return del();
		}));
	}

	template<typename T>
	T Task<T>::Result()
	{
		return m_Result.Get();
	}

	template<typename T>
	void Task<T>::Wait()
	{
		m_Result.Wait();
	}

	template<typename T>
	TaskStatus Task<T>::WaitFor(double seconds)
	{
		std::future_status status = m_Result.WaitFor(seconds);
		if (status == std::future_status::ready)
		{
			return TaskStatus::Ready;
		}
		return TaskStatus::Timeout;
	}

	template<typename T>
	template<typename DelegateT, typename TNewResult>
	Task<TNewResult> Task<T>::ContinueWith(DelegateT func)
	{
		return Task<TNewResult>(std::move(func), *this);
	}

	template<typename T>
	template<typename DelegateT>
	void Task<T>::ContinueWithOnMainThread(DelegateT func)
	{
		ContinueWith([func{ std::move(func) }](T value) mutable
		{
			EventManager::Get().Bus().Emit(Events::Internal.AsyncTaskCompleted, TaskCompleted<T>(std::move(value), std::move(func)));
		});
	}

}