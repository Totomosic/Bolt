#include "EventManager.h"

namespace Bolt
{

	EventManager::EventInfo EventManager::s_EventQueue[MAX_EVENTS];
	id_t EventManager::s_Tail = 0;
	std::unordered_map<id_t, std::vector<EventManager::EventListener>> EventManager::s_Listeners = std::unordered_map<id_t, std::vector<EventManager::EventListener>>();
	std::unordered_map<id_t, id_t> EventManager::s_ListenerMap = std::unordered_map<id_t, id_t>();

	IdManager<id_t> EventManager::s_ListenerIdManager = IdManager<id_t>(0, EventManager::IGNORE_INSTANCE_ID - 1);
	IdManager<id_t> EventManager::s_InstanceIdManager = IdManager<id_t>(0, EventManager::IGNORE_INSTANCE_ID - 1);

	id_t EventManager::GetNextInstanceId()
	{
		return s_InstanceIdManager.GetNextId();
	}

	void EventManager::ReleaseInstanceId(id_t id)
	{
		s_InstanceIdManager.ReleaseId(id);
	}

	id_t EventManager::Subscribe(id_t eventId, const EventManager::Listener& listener, id_t instanceId)
	{
		id_t listenerId = FindNextListenerId();
		id_t listenerIndex = s_Listeners[eventId].size();
		s_Listeners.at(eventId).push_back({ listener, instanceId, listenerId });
		s_ListenerMap[listenerId] = eventId;
		return listenerId;
	}

	void EventManager::Unsubscribe(id_t listenerId)
	{
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

	void EventManager::Post(id_t eventId, id_t instanceId, std::unique_ptr<EventArgs>&& args)
	{
		BLT_ASSERT(s_Tail < MAX_EVENTS, "Event overflow");
		s_EventQueue[s_Tail++] = { eventId, instanceId, std::move(args) };
	}

	void EventManager::FlushEvents()
	{
		for (id_t i = 0; i < s_Tail; i++)
		{
			EventInfo& e = s_EventQueue[i];
			for (auto& listener : s_Listeners[e.EventId])
			{
				if (e.InstanceId == listener.InstanceId || listener.InstanceId == EventManager::IGNORE_INSTANCE_ID)
				{
					listener.Callback(e.EventId, e.Args.get());
				}
			}
		}
		s_Tail = 0;
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