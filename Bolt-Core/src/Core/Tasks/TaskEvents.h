#pragma once
#include "../Events/Event.h"

namespace Bolt
{

	class BLT_API TaskCompletedEvent : public Event 
	{
	public:
		virtual void Execute() = 0;
	};

	template<typename T>
	class BLT_API TaskCompleted : public TaskCompletedEvent
	{
	public:
		using Func = std::function<void(T)>;

	public:
		T Value;
		Func Callback;

	public:
		TaskCompleted()
		{
			
		}

		TaskCompleted(T value, Func callback)
			: Value(std::move(value)), Callback(std::move(callback))
		{
			
		}

		void Execute() override
		{
			Callback(std::move(Value));
		}

	};

}