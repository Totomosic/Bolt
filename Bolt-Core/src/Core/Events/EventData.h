#pragma once
#include "bltpch.h"

namespace Bolt
{

	class BLT_API EventContainer
	{
	public:
		bool Handled = false;
	};

	template<typename T>
	class BLT_API Event : public EventContainer
	{
	public:
		T Data;

	public:
		Event(const T& data)
			: Data(data)
		{
		
		}

		Event(T&& data)
			: Data(std::move(data))
		{
		
		}
	};

	template<>
	class BLT_API Event<void> : public EventContainer
	{

	};

	class BLT_API EventListenerContainer
	{
	public:
		virtual void Emit(EventContainer& e) = 0;
	};

	template<typename T>
	class BLT_API EventListener : public EventListenerContainer
	{
	public:
		using callback_t = std::function<void(Event<T>&)>;

	private:
		callback_t m_Callback;

	public:
		EventListener(const callback_t& callback)
			: m_Callback(callback)
		{
		
		}

		virtual void Emit(EventContainer& e) override
		{
			m_Callback((Event<T>&)e);
		}
	};

}