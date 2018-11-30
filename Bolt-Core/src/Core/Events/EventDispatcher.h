#pragma once
#include "EventManager.h"

namespace Bolt
{

	// Used as an event endpoint, T = Child of EventArgs
	template<typename T = EventArgs>
	class BLT_API EventDispatcher
	{
	public:
		using Listener = std::function<void(id_t, const T*)>;

	private:
		id_t m_InstanceId = EventManager::IGNORE_INSTANCE_ID;
		id_t m_EventId = EventManager::IGNORE_INSTANCE_ID;
		id_t m_ListenerId = EventManager::IGNORE_INSTANCE_ID;
		std::unordered_map<id_t, Listener> m_Listeners;
		IdManager<id_t> m_IdManager = IdManager<id_t>(0, 0);

	public:
		EventDispatcher()
			: m_InstanceId(EventManager::IGNORE_INSTANCE_ID), m_EventId(EventManager::IGNORE_INSTANCE_ID), m_ListenerId(EventManager::IGNORE_INSTANCE_ID), m_Listeners(), m_IdManager(0, 0)
		{
		
		}

		EventDispatcher(id_t eventId)
			: m_InstanceId(EventManager::GetNextInstanceId()), m_EventId(eventId), m_ListenerId(EventManager::Subscribe(m_EventId, std::bind(&EventDispatcher::__EventCallback, this, std::placeholders::_1, std::placeholders::_2), m_InstanceId)), m_Listeners(), m_IdManager(0, EventManager::IGNORE_INSTANCE_ID - 1)
		{
		
		}

		EventDispatcher(const EventDispatcher<T>& other)
			: m_InstanceId(EventManager::GetNextInstanceId()), m_EventId(other.m_EventId), m_ListenerId(EventManager::Subscribe(m_EventId, std::bind(&EventDispatcher::__EventCallback, this, std::placeholders::_1, std::placeholders::_2), m_InstanceId)), m_Listeners(other.m_Listeners), m_IdManager(other.m_IdManager)
		{
		
		}

		EventDispatcher<T>& operator=(const EventDispatcher<T>& other)
		{
			m_InstanceId = EventManager::GetNextInstanceId();
			m_EventId = other.m_EventId;
			m_ListenerId = EventManager::Subscribe(m_EventId, std::bind(&EventDispatcher::__EventCallback, this, std::placeholders::_1, std::placeholders::_2), m_InstanceId);
			m_Listeners = other.m_Listeners;
			m_IdManager = other.m_IdManager;
			return *this;
		}

		EventDispatcher(EventDispatcher<T>&& other)
		{
			m_ListenerId = other.m_ListenerId;
			other.m_ListenerId = EventManager::IGNORE_INSTANCE_ID;
			m_InstanceId = other.m_InstanceId;
			other.m_InstanceId = EventManager::IGNORE_INSTANCE_ID;
			m_EventId = other.m_EventId;
			other.m_EventId = EventManager::IGNORE_INSTANCE_ID;
			m_Listeners = std::move(other.m_Listeners);
			m_IdManager = std::move(other.m_IdManager);

			EventManager::UpdateListener(m_ListenerId, std::bind(&EventDispatcher::__EventCallback, this, std::placeholders::_1, std::placeholders::_2));
		}

		EventDispatcher<T>& operator=(EventDispatcher<T>&& other)
		{
			id_t listenerId = m_ListenerId;
			m_ListenerId = other.m_ListenerId;
			other.m_ListenerId = listenerId;
			id_t instanceId = m_InstanceId;
			m_InstanceId = other.m_InstanceId;
			other.m_InstanceId = instanceId;
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
			if (m_ListenerId != EventManager::IGNORE_INSTANCE_ID && m_InstanceId != EventManager::IGNORE_INSTANCE_ID)
			{
				EventManager::Unsubscribe(m_ListenerId);
				EventManager::ReleaseInstanceId(m_InstanceId);
			}
		}

		id_t EventId() const
		{
			return m_EventId;
		}

		id_t InstanceId() const
		{
			return m_InstanceId;
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

		// Post a new event with given args from this specific instance
		void Post(std::unique_ptr<T>&& args = nullptr)
		{
			EventManager::Post(m_EventId, m_InstanceId, std::move(args));
		}

		void Destroy()
		{
			if (m_ListenerId != EventManager::IGNORE_INSTANCE_ID && m_InstanceId != EventManager::IGNORE_INSTANCE_ID)
			{
				EventManager::Unsubscribe(m_ListenerId);
				EventManager::ReleaseInstanceId(m_InstanceId);
			}
			m_ListenerId = EventManager::IGNORE_INSTANCE_ID;
			m_InstanceId = EventManager::IGNORE_INSTANCE_ID;
		}

		void __EventCallback(id_t eventId, const EventArgs* args)
		{
			int size = m_Listeners.size();
			auto it = m_Listeners.begin();
			for (int i = 0; i < size; i++)
			{
				it->second(eventId, (const T*)args);
				it++;
			}
		}

	};

}