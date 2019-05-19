#pragma once
#include "bltpch.h"

namespace Bolt
{

	class TimerCallbackContainer
	{
	public:
		virtual void operator()() = 0;
	};

	template<typename FuncType>
	class TimerCallback : public TimerCallbackContainer
	{
	private:
		FuncType m_FuncObject;

	public:
		TimerCallback(FuncType funcObj) : TimerCallbackContainer(),
			m_FuncObject(std::move(funcObj))
		{
		
		}

		void operator()() override
		{
			return m_FuncObject();
		}

	};

}