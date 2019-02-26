#pragma once
#include "TaskResult.h"
#include "TaskEvents.h"

namespace Bolt
{

	template<class TResult>
	class BLT_API Task
	{
	private:
		TaskResult<TResult> m_Result;

	private:
		template<typename DelegateT, typename TaskResultT>
		Task(DelegateT func, Task<TaskResultT>& taskResult)
		{
			m_Result = LaunchAsync<TResult>(std::function<TResult()>([taskResult = std::move(taskResult.m_Result), del = std::move(func)]()->TResult
			{
				TaskResult<TaskResultT>& tResult = *(TaskResult<TaskResultT>*)&taskResult;
				return del(tResult.Get());
			}));
		}

		template<typename DelegateT>
		Task(DelegateT func, Task<void>& taskResult)
		{
			m_Result = LaunchAsync<TResult>(std::function<TResult()>([taskResult = std::move(taskResult.m_Result), del = std::move(func)]()->TResult
			{
				TaskResult<int>& tResult = *(TaskResult<int>*)&taskResult;
				tResult.Get();
				return del();
			}));
		}

	public:
		template<typename DelegateT>
		Task(DelegateT func)
		{
			m_Result = LaunchAsync<TResult>(std::function<TResult()>([del = std::move(func)]() -> TResult
			{
				return del();
			}));
		}

		bool IsComplete() const
		{
			return m_Result.m_State->IsFinished.load();
		}

		TResult Result()
		{
			return m_Result.Get();
		}

		void Wait()
		{
			Result();
		}

		template<typename DelegateT, typename TNewResult = std::result_of<DelegateT(TResult)>::type>
		Task<TNewResult> ContinueWith(DelegateT func)
		{
			return Task<TNewResult>(std::move(func), *this);
		}

		template<typename DelegateT>
		void ContinueWithOnMainThread(DelegateT func)
		{
			ContinueWith([func{ std::move(func) }](TResult value)
			{
				EventManager::Post(TaskCompleted<TResult>(std::move(value), std::move(func)));
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
			m_Result = LaunchAsync<int>(std::function<int()>([taskResult = std::move(taskResult.m_Result), del = std::move(func)]()->int
			{
				TaskResult<TaskResultT>& tResult = *(TaskResult<TaskResultT>*)&taskResult;				
				del(tResult.Get());
				return 0;
			}));
		}

		template<typename DelegateT>
		Task(DelegateT func, Task<void>& taskResult)
		{
			m_Result = LaunchAsync<int>(std::function<int()>([taskResult = std::move(taskResult.m_Result), del = std::move(func)]()->int
			{
				TaskResult<int>& tResult = *(TaskResult<int>*)&taskResult;
				tResult.Get();
				del();
				return 0;
			}));
		}

	public:
		template<typename DelegateT>
		Task(DelegateT func)
		{
			m_Result = LaunchAsync<int>(std::function<int()>([del = std::move(func)]()->int
			{
				del();
				return 0;
			}));
		}

		bool IsComplete() const
		{
			return m_Result.m_State->IsFinished.load();
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
			ContinueWith([func{ std::move(func) }]()
			{
				EventManager::Post(TaskCompleted<int>(0, [func{ std::move(func) }](int ignore)
				{
					func();
				}));
			});
		}

		template<typename> friend class Task;
	};

}