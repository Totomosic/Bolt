#pragma once
#include "Task.h"

namespace Bolt
{

	class BLT_API TaskManager
	{
	public:
		template<typename DelegateT, typename TResult = std::result_of<DelegateT()>::type>
		static Task<TResult> Run(DelegateT func)
		{
			Task<TResult> task(std::move(func));
			return std::move(task);
		}

	};

}