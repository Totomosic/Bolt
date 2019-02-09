#include "Types.h"
#include "EventManager.h"

#include "../Tasks/TaskEvents.h"

namespace Bolt
{

	std::mutex EventManager::s_EventQueueMutex = std::mutex();
	std::mutex EventManager::s_ListenersMutex = std::mutex();

	EventQueue<EventManager::EventInfo> EventManager::s_EventQueue = EventQueue<EventManager::EventInfo>(EventManager::MAX_EVENTS);
	std::unordered_map<id_t, std::vector<EventManager::EventListenerInfo>> EventManager::s_Listeners = std::unordered_map<id_t, std::vector<EventManager::EventListenerInfo>>();
	std::unordered_map<id_t, id_t> EventManager::s_ListenerMap = std::unordered_map<id_t, id_t>();

	IdManager<id_t> EventManager::s_ListenerIdManager = IdManager<id_t>(0, EventManager::IGNORE_DISPATCHER_ID - 1);
	IdManager<id_t> EventManager::s_DispatcherIdManager = IdManager<id_t>(0, EventManager::IGNORE_DISPATCHER_ID - 1);

	id_t EventManager::GetNextDispatcherId()
	{
		return s_DispatcherIdManager.GetNextId();
	}

	void EventManager::ReleaseDispatcherId(id_t id)
	{
		s_DispatcherIdManager.ReleaseId(id);
	}

	void EventManager::Unsubscribe(id_t listenerId)
	{
		std::scoped_lock<std::mutex> lock(s_ListenersMutex);
		BLT_ASSERT(s_ListenerMap.find(listenerId) != s_ListenerMap.end(), "Unable to find listener with Id " + std::to_string(listenerId));
		id_t eventId = s_ListenerMap.at(listenerId);
		std::vector<EventListenerInfo>& listenersVector = s_Listeners.at(eventId);
		auto it = std::find_if(listenersVector.begin(), listenersVector.end(), [listenerId](EventListenerInfo& eListener)
		{	
			return eListener.ListenerId == listenerId;
		});
		listenersVector.erase(it);
		ReleaseListenerId(listenerId);
	}

	void EventManager::Post(id_t eventId, id_t dispatcherId)
	{
		Post<Event>(eventId, dispatcherId, std::unique_ptr<Event>());
	}

	void EventManager::Post(id_t eventId)
	{
		Post(eventId, EventManager::IGNORE_DISPATCHER_ID);
	}

	void EventManager::FlushEvents()
	{
		EventInfo* eventQueue;
		id_t eventQueueLength;
		{
			std::scoped_lock<std::mutex, std::mutex> lock(s_EventQueueMutex, s_ListenersMutex);
			eventQueue = s_EventQueue.GetQueuePtr();
			eventQueueLength = s_EventQueue.EventCount();
			s_EventQueue.SwapQueues();
		}
		for (id_t i = 0; i < eventQueueLength; i++)
		{
			EventInfo& e = eventQueue[i];
			for (auto& listener : s_Listeners[e.EventId])
			{
				if (listener.DispatcherId == EventManager::IGNORE_DISPATCHER_ID || e.DispatcherId == listener.DispatcherId)
				{
					ListenerResponse response = (*listener.Callback)(listener.ListenerId, *e.Args);
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
		return s_ListenerIdManager.GetNextId();
	}

	void EventManager::ReleaseListenerId(id_t id)
	{
		s_ListenerIdManager.ReleaseId(id);
	}

	void EventManager::Initialize()
	{
		Subscribe(Events::TASK_CONTINUE_ON_MAIN_THREAD, [](id_t listenerId, Event& eArgs)
		{
			TaskCompletedEvent& e = *(TaskCompletedEvent*)&eArgs;
			e.Execute();
			ListenerResponse response;
			return response;
		});
	}

	id_t EventManager::Subscribe(id_t eventId, std::unique_ptr<EventListenerContainer<Event>> listener, id_t dispatcherId)
	{
		std::scoped_lock<std::mutex> lock(s_ListenersMutex);
		id_t listenerId = FindNextListenerId();
		id_t listenerIndex = s_Listeners[eventId].size();
		s_Listeners.at(eventId).push_back({ std::move(listener), dispatcherId, listenerId });
		s_ListenerMap[listenerId] = eventId;
		return listenerId;
	}

	void EventManager::UpdateListener(id_t listenerId, std::unique_ptr<EventListenerContainer<Event>> listener)
	{
		std::scoped_lock<std::mutex> lock(s_ListenersMutex);
		BLT_ASSERT(s_ListenerMap.find(listenerId) != s_ListenerMap.end(), "Unable to find listener with Id " + std::to_string(listenerId));
		id_t eventId = s_ListenerMap.at(listenerId);
		std::vector<EventListenerInfo>& listenersVector = s_Listeners.at(eventId);
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