#pragma once
#include "Event.h"

namespace Bolt
{

	struct BLT_API ListenerResponse
	{
	public:
		bool HandledEvent = true;
		bool UnsubscribeListener = false;
	};

	template<typename T>
	class BLT_API EventListenerContainer
	{
	public:
		virtual ListenerResponse operator()(id_t listenerId, T& e) = 0;
	};

	template<typename FuncType, typename EventType>
	class BLT_API EventListener : public EventListenerContainer<EventType>
	{
	private:
		FuncType m_FuncObject;

	public:
		EventListener(FuncType funcObj) : EventListenerContainer<EventType>(),
			m_FuncObject(std::move(funcObj))
		{
			
		}

		ListenerResponse operator()(id_t listenerId, EventType& e) override
		{
			static_assert(std::is_same<std::result_of<FuncType(id_t, EventType&)>::type, ListenerResponse>::value, "FuncObject must return ListenerResponse");
			return m_FuncObject(listenerId, e);
		}

	};

}