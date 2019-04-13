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

	public:
		// Max events that can be queued per frame
		static constexpr uint MAX_EVENTS = 500;
		// Create user events with this Id, eg. USER_EVENT = EventManager::USER_EVENT_ID + 0
		static constexpr id_t USER_EVENT_ID = 100000;
		// Use to receive events from all instances posting an event
		static constexpr id_t IGNORE_DISPATCHER_ID = (id_t)-1;

	public:
		static std::unique_ptr<EventManager> s_Instance;

		static EventManager& Get();

	private:
		std::mutex m_EventQueueMutex;
		std::mutex m_ListenersMutex;

		EventQueue<EventInfo> m_EventQueue;
		std::unordered_map<id_t, std::vector<EventListenerInfo>> m_Listeners;
		std::unordered_map<id_t, id_t> m_ListenerMap;

		IdManager<id_t> m_ListenerIdManager;
		IdManager<id_t> m_DispatcherIdManager;

	public:
		EventManager();
		EventManager(const EventManager& other) = delete;
		EventManager& operator=(const EventManager& other) = delete;
		EventManager(EventManager&& other) = delete;
		EventManager& operator=(EventManager&& other) = delete;
		~EventManager() = default;

		// Get a new Id for an instance
		id_t GetNextDispatcherId();
		// Release instance Id
		void ReleaseDispatcherId(id_t id);
		
		// Register a listener to a specific event from an dispatcher with given id, returns a Listener Id
		template<typename EventType>
		id_t Subscribe(std::function<ListenerResponse(EventType&)> listener, id_t dispatcherId)
		{
			return Subscribe(EventType::BLT_EVENT_ID, (std::unique_ptr<EventListenerContainer<Event>>)std::make_unique<EventListener<Event>>([listener{ std::move(listener) }](Event& e)
			{
				return listener((EventType&)e);
			}), dispatcherId);
		}

		// Register a listener to a specific event from any dispatcher, returns a listener Id
		template<typename EventType>
		id_t Subscribe(std::function<ListenerResponse(EventType&)> listener)
		{
			return Subscribe<EventType>(std::move(listener), EventManager::IGNORE_DISPATCHER_ID);
		}

		// Stop a listener from receiving events
		void Unsubscribe(id_t listenerId);

		// Update a listener
		template<typename EventType>
		void UpdateListener(id_t listenerId, std::function<ListenerResponse(EventType&)> listener)
		{
			UpdateListener(listenerId, (std::unique_ptr<EventListenerContainer<Event>>)std::make_unique<EventListener<Event>>([listener{ std::move(listener) }](Event& e)
			{
				return listener((EventType&)e);
			}));
		}

		// Post a new event with given args from a specific dispatcher
		template<typename EventType>
		void PostFromDispatcher(id_t dispatcherId, EventType args)
		{
			std::unique_ptr<EventType> e = std::make_unique<EventType>(std::move(args));
			PostFromDispatcher(EventType::BLT_EVENT_ID, dispatcherId, std::move(e));
		}

		// Post a new event with given args
		template<typename EventType>
		void Post(EventType args)
		{
			PostFromDispatcher(EventManager::IGNORE_DISPATCHER_ID, std::move(args));
		}

		template<typename EventType>
		void PostFromDispatcher(id_t dispatcherId)
		{
			PostFromDispatcher<Event>(EventType::BLT_EVENT_ID, dispatcherId, std::unique_ptr<Event>());
		}

		template<typename EventType>
		void Post()
		{
			PostFromDispatcher<EventType>(EventManager::IGNORE_DISPATCHER_ID);
		}

		// Called automatically every frame before Application::Update() runs, processes all events from previous frame
		void FlushEvents();
		id_t FindNextListenerId();
		void ReleaseListenerId(id_t id);

	private:
		id_t Subscribe(id_t eventId, std::unique_ptr<EventListenerContainer<Event>> listener, id_t dispatcherId);
		void UpdateListener(id_t listenerId, std::unique_ptr<EventListenerContainer<Event>> listener);
		// Post a new event with given args from a specific dispatcher
		template<typename EventType>
		void PostFromDispatcher(id_t eventId, id_t dispatcherId, std::unique_ptr<EventType> args)
		{
			std::scoped_lock<std::mutex> lock(m_EventQueueMutex);
			m_EventQueue.AddEvent(EventInfo{ eventId, dispatcherId, std::move(args) });
		}

	};

}