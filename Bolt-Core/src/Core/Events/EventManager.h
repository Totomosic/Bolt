#pragma once
#include "Event.h"
#include "IdManager.h"
#include "EventQueue.h"
#include "EventListener.h"

namespace Bolt
{

	class BLT_API EventManager
	{
	public:
		// Event handler function, returns whether the event has been handled and should not be passed on to other listeners

		struct BLT_API EventListenerInfo
		{
		public:
			std::unique_ptr<EventListenerContainer<Event>> Callback;
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

		/*struct BLT_API ListenerInfo
		{
		public:
			id_t EventId;
			Listener ListenerPtr;
		};*/		

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
		static std::unordered_map<id_t, std::vector<EventListenerInfo>> s_Listeners;
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
		template<typename FuncType>
		static id_t Subscribe(id_t eventId, FuncType listener, id_t dispatcherId)
		{
			return Subscribe(eventId, (std::unique_ptr<EventListenerContainer<Event>>)std::make_unique<EventListener<FuncType, Event>>(std::move(listener)), dispatcherId);
		}

		// Register a listener to a specific event from any dispatcher, returns a listener Id
		template<typename FuncType>
		static id_t Subscribe(id_t eventId, FuncType listener)
		{
			return Subscribe(eventId, std::move(listener), EventManager::IGNORE_DISPATCHER_ID);
		}

		// Stop a listener from receiving events
		static void Unsubscribe(id_t listenerId);

		// Update a listener
		template<typename FuncType>
		static void UpdateListener(id_t listenerId, FuncType listener)
		{
			UpdateListener(listenerId, (std::unique_ptr<EventListenerContainer<Event>>)std::make_unique<EventListener<FuncType, Event>>(std::move(listener)));
		}

		// Post a new event with given args from a specific dispatcher
		template<typename EventType>
		static void Post(id_t eventId, id_t dispatcherId, EventType args)
		{
			std::unique_ptr<EventType> e = std::make_unique<EventType>();
			*e = std::move(args);
			Post(eventId, dispatcherId, std::move(e));
		}

		// Post a new event with given args
		template<typename EventType>
		static void Post(id_t eventId, EventType args)
		{
			Post(eventId, EventManager::IGNORE_DISPATCHER_ID, std::move(args));
		}

		static void Post(id_t eventId, id_t dispatcherId);
		static void Post(id_t eventId);

		// Called automatically every frame before Application::Update() runs, processes all events from previous frame
		static void FlushEvents();
		static id_t FindNextListenerId();
		static void ReleaseListenerId(id_t id);

		friend class Initializer;

	private:
		static void Initialize();

		static id_t Subscribe(id_t eventId, std::unique_ptr<EventListenerContainer<Event>> listener, id_t dispatcherId);
		static void UpdateListener(id_t listenerId, std::unique_ptr<EventListenerContainer<Event>> listener);
		// Post a new event with given args from a specific dispatcher
		template<typename EventType>
		static void Post(id_t eventId, id_t dispatcherId, std::unique_ptr<EventType> args)
		{
			std::scoped_lock<std::mutex> lock(s_EventQueueMutex);
			s_EventQueue.AddEvent(EventInfo{ eventId, dispatcherId, std::move(args) });
		}

	};

}