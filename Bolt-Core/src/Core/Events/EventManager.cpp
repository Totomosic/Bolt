#include "Types.h"
#include "EventManager.h"

#include "../Tasks/TaskEvents.h"

namespace Bolt
{

	std::unique_ptr<EventManager> EventManager::s_Instance;

	EventManager& EventManager::Get()
	{
		if (!s_Instance)
		{
			s_Instance = std::make_unique<EventManager>();
		}
		return *s_Instance;
	}

	EventManager::EventManager()
		: m_EventQueueMutex(), m_ListenersMutex(), m_EventQueue(EventManager::MAX_EVENTS), m_Listeners(), m_ListenerMap(), m_ListenerIdManager(0, EventManager::IGNORE_DISPATCHER_ID - 1), m_DispatcherIdManager(0, EventManager::IGNORE_DISPATCHER_ID - 1)
	{
		Subscribe<TaskCompletedEvent>([](TaskCompletedEvent & e)
			{
				e.Execute();
				ListenerResponse response;
				return response;
			});
	}

	id_t EventManager::GetNextDispatcherId()
	{
		return m_DispatcherIdManager.GetNextId();
	}

	void EventManager::ReleaseDispatcherId(id_t id)
	{
		m_DispatcherIdManager.ReleaseId(id);
	}

	void EventManager::Unsubscribe(id_t listenerId)
	{
		std::scoped_lock<std::mutex> lock(m_ListenersMutex);
		BLT_ASSERT(m_ListenerMap.find(listenerId) != m_ListenerMap.end(), "Unable to find listener with Id " + std::to_string(listenerId));
		id_t eventId = m_ListenerMap.at(listenerId);
		std::vector<EventListenerInfo>& listenersVector = m_Listeners.at(eventId);
		auto it = std::find_if(listenersVector.begin(), listenersVector.end(), [listenerId](EventListenerInfo& eListener)
		{	
			return eListener.ListenerId == listenerId;
		});
		listenersVector.erase(it);
		ReleaseListenerId(listenerId);
	}

	void EventManager::FlushEvents()
	{
		EventInfo* eventQueue;
		id_t eventQueueLength;
		{
			std::scoped_lock<std::mutex, std::mutex> lock(m_EventQueueMutex, m_ListenersMutex);
			eventQueue = m_EventQueue.GetQueuePtr();
			eventQueueLength = m_EventQueue.EventCount();
			m_EventQueue.SwapQueues();
		}
		for (id_t i = 0; i < eventQueueLength; i++)
		{
			EventInfo& e = eventQueue[i];
			for (auto& listener : m_Listeners[e.EventId])
			{
				if (listener.DispatcherId == EventManager::IGNORE_DISPATCHER_ID || e.DispatcherId == listener.DispatcherId)
				{
					ListenerResponse response = (*listener.Callback)(*e.Args);
					if (response.UnsubscribeListener)
					{
						Unsubscribe(listener.ListenerId);
					}
					if (response.HandledEvent)
					{
						// Event has already been handled and should not be propogated to other event listeners
						break;
					}
				}
			}
		}
	}

	id_t EventManager::FindNextListenerId()
	{
		return m_ListenerIdManager.GetNextId();
	}

	void EventManager::ReleaseListenerId(id_t id)
	{
		m_ListenerIdManager.ReleaseId(id);
	}

	id_t EventManager::Subscribe(id_t eventId, std::unique_ptr<EventListenerContainer<Event>> listener, id_t dispatcherId)
	{
		std::scoped_lock<std::mutex> lock(m_ListenersMutex);
		id_t listenerId = FindNextListenerId();
		id_t listenerIndex = m_Listeners[eventId].size();
		m_Listeners.at(eventId).push_back({ std::move(listener), dispatcherId, listenerId });
		m_ListenerMap[listenerId] = eventId;
		return listenerId;
	}

	void EventManager::UpdateListener(id_t listenerId, std::unique_ptr<EventListenerContainer<Event>> listener)
	{
		std::scoped_lock<std::mutex> lock(m_ListenersMutex);
		BLT_ASSERT(m_ListenerMap.find(listenerId) != m_ListenerMap.end(), "Unable to find listener with Id " + std::to_string(listenerId));
		id_t eventId = m_ListenerMap.at(listenerId);
		std::vector<EventListenerInfo>& listenersVector = m_Listeners.at(eventId);
		for (EventListenerInfo& l : listenersVector)
		{
			if (l.ListenerId == listenerId)
			{
				l.Callback = std::move(listener);
				break;
			}
		}
	}

}