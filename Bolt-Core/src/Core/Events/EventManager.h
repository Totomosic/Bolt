#pragma once
#include "EventArgs.h"
#include "IdManager.h"

namespace Bolt
{

	class BLT_API EventManager
	{
	public:
		using Listener = std::function<void(id_t, const EventArgs*)>;

		struct BLT_API EventListener
		{
		public:
			Listener Callback;
			id_t InstanceId;
			id_t ListenerId;
		};

		struct BLT_API EventInfo
		{
		public:
			id_t EventId;
			id_t InstanceId;
			std::unique_ptr<EventArgs> Args;
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
		// Use to receive events from all instances Posting an event
		static constexpr id_t IGNORE_INSTANCE_ID = (id_t)-1;

	private:
		static EventInfo s_EventQueue[MAX_EVENTS];
		static id_t s_Tail;
		static std::unordered_map<id_t, std::vector<EventListener>> s_Listeners;
		static std::unordered_map<id_t, id_t> s_ListenerMap;

		static IdManager<id_t> s_ListenerIdManager;
		static IdManager<id_t> s_InstanceIdManager;

	public:
		EventManager() = delete;

		// Get a new Id for an instance
		static id_t GetNextInstanceId();
		// Release instance Id
		static void ReleaseInstanceId(id_t id);
		
		// Register a listener to a specific event, returns a Listener Id
		static id_t Subscribe(id_t eventId, const Listener& listener, id_t instanceId = IGNORE_INSTANCE_ID);
		// Stop a listener from receiving events
		static void Unsubscribe(id_t listenerId);
		// Update a listener
		static void UpdateListener(id_t listenerId, const Listener& listener);

		// Post a new event with given args from a specific instance
		static void Post(id_t eventId, id_t instanceId = IGNORE_INSTANCE_ID, std::unique_ptr<EventArgs>&& args = nullptr);

		// Called automatically every frame before Application::Update() runs, processes all events from previous frame
		static void FlushEvents();

	private:
		static id_t FindNextListenerId();
		static void ReleaseListenerId(id_t id);

	};

}