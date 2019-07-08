#pragma once
#include "EventData.h"
#include "EventManager.h"
#include "EventEmitter.h"
#include "EventBusMount.h"
#include "IdManager.h"

namespace Bolt
{

	class BLT_API EventBusBase
	{
	public:
		virtual void Flush() = 0;
	};

	template<typename EventIdT>
	class BLT_API GenericEventBus : public EventBusBase
	{
	public:
		using EventId = EventIdT;

	private:
		struct BLT_API ListenerLocation
		{
		public:
			EventId Id;
			EventListenerContainer* Listener;
		};

		struct BLT_API EventInfo
		{
		public:
			EventId Id;
			std::unique_ptr<EventContainer> Event;
		};

	private:
		std::mutex m_ListenersMutex;
		std::mutex m_EventsMutex;
		std::unordered_map<EventIdT, std::vector<std::unique_ptr<EventListenerContainer>>> m_Listeners;
		std::unordered_map<uint32_t, ListenerLocation> m_ListenerLocations;
		std::vector<EventInfo> m_Events;
		IdManager<uint32_t> m_ListenerIds;

		std::unique_ptr<EventBusMount> m_MountManager;

	public:
		GenericEventBus()
			: m_ListenersMutex(), m_EventsMutex(), m_Listeners(), m_ListenerLocations(), m_Events(), m_ListenerIds(0, (uint32_t)-1), m_MountManager(std::make_unique<EventBusMount>(this))
		{
			EventManager::Get().AddEventBus(this);
		}

		GenericEventBus(const GenericEventBus<EventIdT>& other) = delete;
		GenericEventBus<EventIdT>& operator=(const GenericEventBus<EventIdT>& other) = delete;

		GenericEventBus(GenericEventBus<EventIdT>&& other)
			: m_ListenersMutex(), m_EventsMutex(), m_Listeners(std::move(other.m_Listeners)), m_ListenerLocations(std::move(other.m_ListenerLocations)), m_Events(std::move(other.m_Events)), 
			  m_ListenerIds(std::move(other.m_ListenerIds)), m_MountManager(std::move(other.m_MountManager))
		{
			m_MountManager->SetEventBus(this);
			EventManager::Get().UpdateEventBus(&other, this);
		}

		GenericEventBus<EventIdT>& operator=(GenericEventBus<EventIdT>&& other)
		{
			m_Listeners = std::move(other.m_Listeners);
			m_ListenerLocations = std::move(other.m_ListenerLocations);
			m_Events = std::move(other.m_Events);
			m_ListenerIds = std::move(other.m_ListenerIds);
			m_MountManager = std::move(other.m_MountManager);
			m_MountManager->SetEventBus(this);
			EventManager::Get().UpdateEventBus(&other, this);
			return *this;
		}

		~GenericEventBus()
		{
			EventManager::Get().RemoveBus(this);
		}

		template<typename T>
		GenericEventEmitter<T, EventIdT> GetEmitter(const EventIdT& eventId)
		{
			return GenericEventEmitter<T, EventIdT>(eventId, *this);
		}

		EventBusMount* GetMount() const
		{
			return m_MountManager.get();
		}

		void MountOn(GenericEventBus<EventIdT>& bus) const
		{
			GetMount()->AddParent(bus.GetMount());
			bus.GetMount()->AddChild(GetMount());
		}

		template<typename T>
		uint32_t On(const EventIdT& eventId, const typename EventListener<T>::callback_t& callback)
		{
			std::scoped_lock<std::mutex> lock(m_ListenersMutex);
			uint32_t id = m_ListenerIds.GetNextId();
			std::unique_ptr<EventListener<T>> listener = std::make_unique<EventListener<T>>(callback);
			m_ListenerLocations[id] = { eventId, listener.get() };
			m_Listeners[eventId].push_back(std::move(listener));
			return id;
		}

		void RemoveEventListener(uint32_t id)
		{
			std::scoped_lock<std::mutex> lock(m_ListenersMutex);
			ListenerLocation& location = m_ListenerLocations.at(id);
			std::vector<std::unique_ptr<EventListenerContainer>>& vector = m_Listeners[location.Id];
			auto it = std::find_if(vector.begin(), vector.end(), [&location](const std::unique_ptr<EventListenerContainer>& container)
				{
					return container.get() == location.Listener;
				});
			if (it != vector.end())
			{
				vector.erase(it);
				m_ListenerLocations.erase(id);
				m_ListenerIds.ReleaseId(id);
			}
		}

		template<typename T>
		void Emit(const EventIdT& eventId, const T& data)
		{
			EmitEvent(eventId, std::make_unique<Event<T>>(data));
		}

		template<typename T>
		void Emit(const EventIdT& eventId, T&& data)
		{
			EmitEvent(eventId, std::make_unique<Event<T>>(std::move(data)));
		}

		void Emit(const EventIdT& eventId)
		{
			EmitEvent(eventId, std::make_unique<Event<void>>());
		}

		virtual void Flush() override
		{
			std::vector<EventInfo> eventQueue;
			{
				std::scoped_lock<std::mutex> eventLock(m_EventsMutex);
				eventQueue = std::move(m_Events);
				m_Events = std::vector<EventInfo>();
			}
			for (EventInfo& event : eventQueue)
			{
				ProcessEvent(event);
				for (EventBusMount* mount : GetMount()->GetChildren())
				{
					// PROCESS EVENT HERE
				}
			}
		}

	private:
		void EmitEvent(const EventIdT& eventId, std::unique_ptr<EventContainer>&& event)
		{
			std::scoped_lock<std::mutex> lock(m_EventsMutex);
			m_Events.push_back({ eventId, std::move(event) });
		}

		void ProcessEvent(EventInfo& e) const
		{
			if (!e.Event->Handled && m_Listeners.find(e.Id) != m_Listeners.end())
			{
				const std::vector<std::unique_ptr<EventListenerContainer>>& listeners = m_Listeners.at(e.Id);
				for (const std::unique_ptr<EventListenerContainer>& listener : listeners)
				{
					listener->Emit(*e.Event);
					if (e.Event->Handled)
					{
						break;
					}
				}
			}
		}

	};

	using EventBus = GenericEventBus<uint32_t>;

}