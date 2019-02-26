#pragma once
#include "EventManager.h"

namespace Bolt
{

	// Used as an event endpoint, T = Subclass of Event
	template<typename T>
	class BLT_API EventDispatcher
	{
	public:
		using Listener = EventListenerContainer<T>;

	private:
		id_t m_DispatcherId;
		id_t m_ListenerId;
		std::unordered_map<id_t, std::unique_ptr<Listener>> m_Listeners;
		IdManager<id_t> m_IdManager;

	public:
		EventDispatcher()
			: m_DispatcherId(EventManager::GetNextDispatcherId()), m_ListenerId(EventManager::Subscribe<T>(std::bind(&EventDispatcher::PrivateEventCallback, this, std::placeholders::_1), m_DispatcherId)), m_Listeners(), m_IdManager(0, EventManager::IGNORE_DISPATCHER_ID - 1)
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
			m_Listeners = std::move(other.m_Listeners);
			m_IdManager = std::move(other.m_IdManager);

			EventManager::UpdateListener<T>(m_ListenerId, std::bind(&EventDispatcher::PrivateEventCallback, this, std::placeholders::_1));
		}

		EventDispatcher<T>& operator=(EventDispatcher<T>&& other)
		{
			id_t listenerId = m_ListenerId;
			m_ListenerId = other.m_ListenerId;
			other.m_ListenerId = listenerId;
			id_t dispatcherId = m_DispatcherId;
			m_DispatcherId = other.m_DispatcherId;
			other.m_DispatcherId = dispatcherId;
			m_Listeners = std::move(other.m_Listeners);
			m_IdManager = std::move(other.m_IdManager);

			EventManager::UpdateListener<T>(m_ListenerId, std::bind(&EventDispatcher::PrivateEventCallback, this, std::placeholders::_1));

			return *this;
		}

		~EventDispatcher()
		{
			Destroy();
		}

		constexpr static id_t EventId()
		{
			return T::BLT_EVENT_ID;
		}

		id_t DispatcherId() const
		{
			return m_DispatcherId;
		}

		template<typename FuncType>
		id_t Subscribe(FuncType listener)
		{
			id_t id = m_IdManager.GetNextId();
			m_Listeners[id] = std::make_unique<EventListener<T, FuncType>>(std::move(listener));
			return id;
		}

		void Unsubscribe(id_t listenerId)
		{
			m_Listeners.erase(listenerId);
			m_IdManager.ReleaseId(listenerId);
		}

		// Post a new event with no args from thsi specific dispatcher
		void Post()
		{
			EventManager::PostFromDispatcher<T>(m_DispatcherId);
		}

		// Post a new event with given args from this specific dispatcher
		void Post(T args)
		{
			EventManager::PostFromDispatcher(m_DispatcherId, std::move(args));
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

		void Clear()
		{
			m_Listeners.clear();
			m_IdManager.Reset();
		}

		ListenerResponse PrivateEventCallback(T& args)
		{
			auto it = m_Listeners.begin();
			bool handled = false;
			while (it != m_Listeners.end())
			{
				ListenerResponse response = (*it->second)(args);				
				if (response.UnsubscribeListener)
				{
					m_Listeners.erase(it);
				}
				else
				{
					it++;
				}
				if (response.HandledEvent)
				{
					// Event was handled and should not be propogated to other event listeners
					handled = true;
					break;
				}
			}
			ListenerResponse response;
			response.HandledEvent = handled;
			return response;
		}

	};

}