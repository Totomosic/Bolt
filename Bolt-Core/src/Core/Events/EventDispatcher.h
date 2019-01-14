#pragma once
#include "EventManager.h"

namespace Bolt
{

	// Used as an event endpoint, T = Subclass of Event
	template<typename T = Event>
	class BLT_API EventDispatcher
	{
	public:
		using Listener = std::function<bool(id_t, T&)>;

	public:
		id_t m_DispatcherId;
		id_t m_EventId;
		id_t m_ListenerId;
		std::unordered_map<id_t, Listener> m_Listeners;
		IdManager<id_t> m_IdManager;

	public:
		EventDispatcher()
			: m_DispatcherId(EventManager::IGNORE_DISPATCHER_ID), m_EventId(EventManager::IGNORE_DISPATCHER_ID), m_ListenerId(EventManager::IGNORE_DISPATCHER_ID), m_Listeners(), m_IdManager(0, EventManager::IGNORE_DISPATCHER_ID - 1)
		{
			
		}

		EventDispatcher(id_t eventId)
			: m_DispatcherId(EventManager::GetNextDispatcherId()), m_EventId(eventId), m_ListenerId(EventManager::Subscribe(m_EventId, std::bind(&EventDispatcher::__EventCallback, this, std::placeholders::_1, std::placeholders::_2), m_DispatcherId)), m_Listeners(), m_IdManager(0, EventManager::IGNORE_DISPATCHER_ID - 1)
		{
			
		}

		EventDispatcher(const EventDispatcher<T>& other) = delete;
		EventDispatcher<T>& operator=(const EventDispatcher<T>& other) = delete;

		EventDispatcher(EventDispatcher<T>&& other)
		{
			m_ListenerId = other.m_ListenerId;
			other.m_ListenerId = EventManager::IGNORE_DISPATCHER_ID;
			m_DispatcherId = other.m_DispatcherId;
			other.m_DispatcherId = EventManager::IGNORE_DISPATCHER_ID;
			m_EventId = other.m_EventId;
			other.m_EventId = EventManager::IGNORE_DISPATCHER_ID;
			m_Listeners = std::move(other.m_Listeners);
			m_IdManager = std::move(other.m_IdManager);

			EventManager::UpdateListener(m_ListenerId, std::bind(&EventDispatcher::__EventCallback, this, std::placeholders::_1, std::placeholders::_2));
		}

		EventDispatcher<T>& operator=(EventDispatcher<T>&& other)
		{
			id_t listenerId = m_ListenerId;
			m_ListenerId = other.m_ListenerId;
			other.m_ListenerId = listenerId;
			id_t dispatcherId = m_DispatcherId;
			m_DispatcherId = other.m_DispatcherId;
			other.m_DispatcherId = dispatcherId;
			id_t eventId = m_EventId;
			m_EventId = other.m_EventId;
			other.m_EventId = eventId;
			m_Listeners = std::move(other.m_Listeners);
			m_IdManager = std::move(other.m_IdManager);

			EventManager::UpdateListener(m_ListenerId, std::bind(&EventDispatcher::__EventCallback, this, std::placeholders::_1, std::placeholders::_2));

			return *this;
		}

		~EventDispatcher()
		{
			if (m_ListenerId != EventManager::IGNORE_DISPATCHER_ID && m_DispatcherId != EventManager::IGNORE_DISPATCHER_ID)
			{
				EventManager::Unsubscribe(m_ListenerId);
				EventManager::ReleaseDispatcherId(m_DispatcherId);
			}
		}

		id_t EventId() const
		{
			return m_EventId;
		}

		id_t DispatcherId() const
		{
			return m_DispatcherId;
		}

		id_t Subscribe(const Listener& listener)
		{
			id_t id = m_IdManager.GetNextId();
			m_Listeners[id] = listener;
			return id;
		}

		void Unsubscribe(id_t listenerId)
		{
			m_Listeners.erase(listenerId);
		}

		// Post a new event with given args from this specific dispatcher
		void Post(std::unique_ptr<T>&& args = nullptr)
		{
			EventManager::Post(m_EventId, m_DispatcherId, std::move(args));
		}

		void Destroy()
		{
			if (m_ListenerId != EventManager::IGNORE_DISPATCHER_ID && m_DispatcherId != EventManager::IGNORE_DISPATCHER_ID)
			{
				EventManager::Unsubscribe(m_ListenerId);
				EventManager::ReleaseDispatcherId(m_DispatcherId);
			}
			m_ListenerId = EventManager::IGNORE_DISPATCHER_ID;
			m_DispatcherId = EventManager::IGNORE_DISPATCHER_ID;
		}

		bool __EventCallback(id_t eventId, Event& args)
		{
			int size = m_Listeners.size();
			auto it = m_Listeners.begin();
			bool handled = false;
			for (int i = 0; i < size; i++)
			{
				if (it->second(eventId, *(T*)&args))
				{
					// Event was handled and should not be propogated to other event listeners
					handled = true;
					break;
				}
				it++;
			}
			return handled;
		}

	};

}