#pragma once
#include "Event.h"
#include "IdManager.h"
#include "EventQueue.h"

namespace Bolt
{

	class BLT_API EventManager
	{
	public:
		// Event handler function, returns whether the event has been handled and should not be passed on to other listeners
		using Listener = std::function<bool(id_t, Event&)>;

		struct BLT_API EventListener
		{
		public:
			Listener Callback;
			id_t DispatcherId;
			id_t ListenerId;
		};

		struct BLT_API EventInfo
		{
		public:
			id_t EventId;
			id_t DispatcherId;
			std::unique_ptr<Event> Args;
		};

		struct BLT_API ListenerInfo
		{
		public:
			id_t EventId;
			Listener ListenerPtr;
		};		

	public:
		// Max events that can be queued per frame
		static constexpr uint MAX_EVENTS = 500;
		// Create user events with this Id, eg. USER_EVENT = EventManager::USER_EVENT_ID + 0
		static constexpr id_t USER_EVENT_ID = 100000;
		// Use to receive events from all instances posting an event
		static constexpr id_t IGNORE_DISPATCHER_ID = (id_t)-1;

	private:
		static std::mutex s_EventQueueMutex;
		static std::mutex s_ListenersMutex;

		static EventQueue<EventInfo> s_EventQueue;
		static std::unordered_map<id_t, std::vector<EventListener>> s_Listeners;
		static std::unordered_map<id_t, id_t> s_ListenerMap;

		static IdManager<id_t> s_ListenerIdManager;
		static IdManager<id_t> s_DispatcherIdManager;

	public:
		EventManager() = delete;

		// Get a new Id for an instance
		static id_t GetNextDispatcherId();
		// Release instance Id
		static void ReleaseDispatcherId(id_t id);
		
		// Register a listener to a specific event from an dispatcher with given id, returns a Listener Id
		static id_t Subscribe(id_t eventId, const Listener& listener, id_t dispatcherId);
		// Register a listener to a specific event from any dispatcher, returns a listener Id
		static id_t Subscribe(id_t eventId, const Listener& listener);
		// Stop a listener from receiving events
		static void Unsubscribe(id_t listenerId);
		// Update a listener
		static void UpdateListener(id_t listenerId, const Listener& listener);

		// Post a new event with given args from a specific dispatcher
		static void Post(id_t eventId, id_t dispatcherId, std::unique_ptr<Event>&& args = nullptr);
		// Post a new event with given args
		static void Post(id_t eventId, std::unique_ptr<Event>&& args = nullptr);

		// Called automatically every frame before Application::Update() runs, processes all events from previous frame
		static void FlushEvents();
		static id_t FindNextListenerId();
		static void ReleaseListenerId(id_t id);

	};

}