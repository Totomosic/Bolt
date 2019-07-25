#pragma once
#include "bltpch.h"

namespace Bolt
{

	template<typename F>
	class BLT_API SharedFunction
	{
	private:
		std::shared_ptr<F> m_Function;

	public:
		SharedFunction() {}
		SharedFunction(F&& f)
			: m_Function(std::make_shared<F>(std::move(f)))
		{
		
		}
		
		template<typename... Args>
		auto operator()(Args&& ... args) const
		{
			return (*m_Function)(std::forward<Args>(args)...);
		}
	};

	template<typename F>
	SharedFunction<std::decay_t<F>> make_shared_function(F&& f)
	{
		return { std::forward<F>(f) };
	}

}