#pragma once
#include "Event.h"

namespace Bolt
{

	template<typename T>
	class BLT_API EventListenerContainer
	{
	public:
		virtual bool operator()(id_t listenerId, T& e) = 0;
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

		bool operator()(id_t listenerId, EventType& e) override
		{
			return m_FuncObject(listenerId, e);
		}

	};

}