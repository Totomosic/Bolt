#pragma once
#include "bltpch.h"

namespace Bolt
{

	template<typename T>
	class BLT_API TaskState
	{
	public:
		T Data;
		std::atomic<bool> IsFinished;
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
			m_State->IsFinished.store(false);
		}

		T Get()
		{
			while (!m_State->IsFinished.load())
			{
				std::this_thread::yield();
			}
			return std::move(m_State->Data);
		}

	};

	template<typename TFuncResult, typename TResult = TFuncResult>
	TaskResult<TResult> LaunchAsync(std::function<TFuncResult()> func)
	{
		TaskResult<TResult> result;
		std::thread t([statePtr = result.m_State, func = std::move(func)]()
		{
			statePtr->Data = func();
			statePtr->IsFinished = true;
		});
		t.detach();
		return result;
	}

}