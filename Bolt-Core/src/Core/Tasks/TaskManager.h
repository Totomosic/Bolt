#pragma once
#include "Task.h"
#include "SharedFunction.h"

namespace Bolt
{

	class BLT_API TaskManager
	{
	public:
		template<typename DelegateT, typename TResult = typename std::result_of<DelegateT()>::type>
		static Task<TResult> Run(DelegateT func)
		{
			Task<TResult> task(std::move(func));
			return task;
		}

	};

}