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
		virtual ListenerResponse operator()(T& e) = 0;
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

		ListenerResponse operator()(EventType& e) override
		{
			return m_FuncObject(e);
		}

	};

}