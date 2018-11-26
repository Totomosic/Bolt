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
		id_t m_InstanceId;
		id_t m_EventId;
		id_t m_ListenerId;
		std::unordered_map<id_t, Listener> m_Listeners;
		IdManager<id_t> m_IdManager;

	public:
		EventDispatcher(id_t eventId)
			: m_InstanceId(EventManager::GetNextInstanceId()), m_EventId(eventId), m_ListenerId(EventManager::Subscribe(m_EventId, std::bind(&EventDispatcher::__EventCallback, this, std::placeholders::_1, std::placeholders::_2), m_InstanceId)), m_Listeners(), m_IdManager(0, EventManager::IGNORE_INSTANCE_ID - 1)
		{
		
		}

		~EventDispatcher()
		{
			EventManager::Unsubscribe(m_ListenerId);
			EventManager::ReleaseInstanceId(m_InstanceId);
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