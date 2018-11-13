#pragma once
#include "EventArgs.h"

namespace Bolt
{

	class BLT_API EventManager
	{
	public:
		using Listener = std::function<void(id_t, const EventArgs*)>;

		struct BLT_API EventInfo
		{
		public:
			id_t EventId;
			std::unique_ptr<EventArgs> Args;
		};

		struct BLT_API ListenerInfo
		{
		public:
			id_t EventId;
			const Listener* ListenerPtr;
		};		

	public:
		// Max events that can be queued per frame
		static constexpr uint MAX_EVENTS = 500;
		// Create user events with this Id, eg. USER_EVENT = EventManager::USER_EVENT_ID + 0
		static constexpr id_t USER_EVENT_ID = 100000;

	private:
		static EventInfo s_EventQueue[MAX_EVENTS];
		static id_t s_Tail;
		static std::unordered_map<id_t, std::vector<Listener>> s_Listeners;
		static std::unordered_map<id_t, ListenerInfo> s_ListenerInfo;

	public:
		EventManager() = delete;
		
		// Register a listener to a specific event, returns a Listener Id
		static id_t Subscribe(id_t eventId, const Listener& listener);
		// Stop a listener from receiving events
		static void Unsubscribe(id_t listenerId);

		// Post a new event with given args
		static void Post(id_t eventId, std::unique_ptr<EventArgs>&& args = nullptr);

		// Called automatically every frame before Application::Update() runs, processes all events from previous frame
		static void FlushEvents();

	private:
		static id_t FindNextListenerId();

	};

}