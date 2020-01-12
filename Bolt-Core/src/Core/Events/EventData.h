#pragma once
#include "bltpch.h"
#include "BoltLib/Profiling/Profiling.h"

namespace Bolt
{

	BLT_API enum class ListenerPriority
	{
		Low,
		Medium,
		High,
		Custom
	};

	class BLT_API EventContainer
	{
	public:
		bool Handled = false;

	public:
		virtual ~EventContainer() {}

		inline void StopPropagation() { Handled = true; }
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
	protected:
		ListenerPriority m_Priority = ListenerPriority::Medium;

	public:
		EventListenerContainer(ListenerPriority priority)
			: m_Priority(priority)
		{
		
		}

		virtual ~EventListenerContainer() {}
		virtual void Emit(EventContainer& e) = 0;	

		inline ListenerPriority GetPriority() const { return m_Priority; }

		template<typename> friend class GenericEventBus;

	};

	template<typename T>
	class BLT_API EventListener : public EventListenerContainer
	{
	public:
		using callback_t = std::function<void(Event<T>&)>;

	private:
		callback_t m_Callback;

	public:
		EventListener(const callback_t& callback, ListenerPriority priority) : EventListenerContainer(priority),
			m_Callback(callback)
		{
		
		}

		virtual void Emit(EventContainer& e) override
		{
			BLT_PROFILE_FUNCTION();
			m_Callback((Event<T>&)e);
		}
	};

}