#include "Types.h"

#include "EventManager.h"

namespace Bolt
{

	std::mutex EventManager::s_EventQueueMutex = std::mutex();
	std::mutex EventManager::s_ListenersMutex = std::mutex();

	EventQueue<EventManager::EventInfo> EventManager::s_EventQueue = EventQueue<EventManager::EventInfo>(EventManager::MAX_EVENTS);
	std::unordered_map<id_t, std::vector<EventManager::EventListener>> EventManager::s_Listeners = std::unordered_map<id_t, std::vector<EventManager::EventListener>>();
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

	id_t EventManager::Subscribe(id_t eventId, const EventManager::Listener& listener, id_t dispatcherId)
	{
		std::scoped_lock<std::mutex> lock(s_ListenersMutex);
		id_t listenerId = FindNextListenerId();
		id_t listenerIndex = s_Listeners[eventId].size();
		s_Listeners.at(eventId).push_back({ listener, dispatcherId, listenerId });
		s_ListenerMap[listenerId] = eventId;
		return listenerId;
	}

	id_t EventManager::Subscribe(id_t eventId, const EventManager::Listener& listener)
	{
		return Subscribe(eventId, listener, EventManager::IGNORE_DISPATCHER_ID);
	}

	void EventManager::Unsubscribe(id_t listenerId)
	{
		std::scoped_lock<std::mutex> lock(s_ListenersMutex);
		BLT_ASSERT(s_ListenerMap.find(listenerId) != s_ListenerMap.end(), "Unable to find listener with Id " + std::to_string(listenerId));
		id_t eventId = s_ListenerMap.at(listenerId);
		std::vector<EventListener>& listenersVector = s_Listeners.at(eventId);
		auto it = std::find_if(listenersVector.begin(), listenersVector.end(), [listenerId](EventListener& eListener)
		{	
			return eListener.ListenerId == listenerId;
		});
		listenersVector.erase(it);
		ReleaseListenerId(listenerId);
	}

	void EventManager::UpdateListener(id_t listenerId, const Listener& listener)
	{
		std::scoped_lock<std::mutex> lock(s_ListenersMutex);
		BLT_ASSERT(s_ListenerMap.find(listenerId) != s_ListenerMap.end(), "Unable to find listener with Id " + std::to_string(listenerId));		
		id_t eventId = s_ListenerMap.at(listenerId);
		std::vector<EventListener>& listenersVector = s_Listeners.at(eventId);
		for (EventListener& l : listenersVector)
		{
			if (l.ListenerId == listenerId)
			{
				l.Callback = listener;
				break;
			}
		}
	}

	void EventManager::Post(id_t eventId, id_t dispatcherId, std::unique_ptr<Event>&& args)
	{
		std::scoped_lock<std::mutex> lock(s_EventQueueMutex);
		BLT_ASSERT(s_EventQueue.EventCount() < MAX_EVENTS, "Event overflow");		
		s_EventQueue.AddEvent({ eventId, dispatcherId, std::move(args) });
	}

	void EventManager::Post(id_t eventId, std::unique_ptr<Event>&& args)
	{
		return Post(eventId, EventManager::IGNORE_DISPATCHER_ID, std::move(args));
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
					if (listener.Callback(listener.ListenerId, *e.Args))
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

}