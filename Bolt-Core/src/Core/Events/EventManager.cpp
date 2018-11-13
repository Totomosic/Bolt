#include "EventManager.h"

namespace Bolt
{

	EventManager::EventInfo EventManager::s_EventQueue[MAX_EVENTS];
	id_t EventManager::s_Tail = 0;
	std::unordered_map<id_t, std::vector<EventManager::Listener>> EventManager::s_Listeners = std::unordered_map<id_t, std::vector<EventManager::Listener>>();
	std::unordered_map<id_t, EventManager::ListenerInfo> EventManager::s_ListenerInfo = std::unordered_map<id_t, EventManager::ListenerInfo>();

	id_t EventManager::Subscribe(id_t eventId, const EventManager::Listener& listener)
	{
		id_t listenerId = FindNextListenerId();
		id_t listenerIndex = s_Listeners[eventId].size();
		s_Listeners.at(eventId).push_back(listener);
		ListenerInfo info;
		info.EventId = eventId;
		info.ListenerPtr = &s_Listeners.at(eventId).at(listenerIndex);
		s_ListenerInfo[listenerId] = info;
		return listenerId;
	}

	void EventManager::Unsubscribe(id_t listenerId)
	{
		BLT_ASSERT(s_ListenerInfo.find(listenerId) != s_ListenerInfo.end(), "Unable to find listener with Id " + std::to_string(listenerId));
		ListenerInfo& info = s_ListenerInfo.at(listenerId);
		std::vector<EventManager::Listener>& listenerVector = s_Listeners.at(info.EventId);
		auto it = std::find_if(listenerVector.begin(), listenerVector.end(), [&info](EventManager::Listener& listener)
		{
			return info.ListenerPtr == &listener;
		});
		BLT_ASSERT(it != listenerVector.end(), "Unable to find listener in vector");
		listenerVector.erase(it);
		s_ListenerInfo.erase(listenerId);
	}

	void EventManager::Post(id_t eventId, std::unique_ptr<EventArgs>&& args)
	{
		BLT_ASSERT(s_Tail < MAX_EVENTS, "Event overflow");
		s_EventQueue[s_Tail++] = { eventId, std::move(args) };
	}

	void EventManager::FlushEvents()
	{
		for (id_t i = 0; i < s_Tail; i++)
		{
			EventInfo& e = s_EventQueue[i];
			for (EventManager::Listener& listener : s_Listeners[e.EventId])
			{
				listener(e.EventId, e.Args.get());
			}
		}
		s_Tail = 0;
	}

	id_t EventManager::FindNextListenerId()
	{
		static id_t currentId = 0;
		return currentId++;
	}

}