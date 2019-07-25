#pragma once
#include "EventData.h"
#include "EventManager.h"
#include "EventEmitter.h"
#include "IdManager.h"

namespace Bolt
{

	// ============================================================================================================================================================================
	// EVENT BUS DECLARATION
	// ============================================================================================================================================================================

	template<typename T>
	class EventBusMount;

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

		std::unique_ptr<EventBusMount<EventIdT>> m_MountManager;

	public:
		GenericEventBus(bool addToEventManager = true);
		GenericEventBus(const GenericEventBus<EventIdT>& other) = delete;
		GenericEventBus<EventIdT>& operator=(const GenericEventBus<EventIdT>& other) = delete;
		GenericEventBus(GenericEventBus<EventIdT>&& other);
		GenericEventBus<EventIdT>& operator=(GenericEventBus<EventIdT>&& other);
		~GenericEventBus();

		template<typename T>
		GenericEventEmitter<T, EventIdT> GetEmitter(const EventIdT& eventId);
		EventBusMount<EventIdT>* GetMount() const;

		void MountOn(GenericEventBus<EventIdT>& bus) const;
		template<typename T>
		uint32_t On(const EventIdT& eventId, const typename EventListener<T>::callback_t& callback);

		void RemoveEventListener(uint32_t id);

		template<typename T>
		void Emit(const EventIdT& eventId, const T& data);
		template<typename T>
		void Emit(const EventIdT& eventId, T&& data);
		void Emit(const EventIdT& eventId);
		virtual void Flush() override;

	private:
		void EmitEvent(const EventIdT& eventId, std::unique_ptr<EventContainer>&& event);
		void ProcessEvent(EventInfo& e) const;

	};

	// ============================================================================================================================================================================
	// EVENT BUS MOUNT DECLARATION
	// ============================================================================================================================================================================

	template<typename EventIdT>
	class BLT_API EventBusMount
	{
	private:
		GenericEventBus<EventIdT>* m_EventBus;
		std::vector<EventBusMount<EventIdT>*> m_Parents;
		std::vector<EventBusMount<EventIdT>*> m_Children;

	public:
		EventBusMount(GenericEventBus<EventIdT>* eventBus);
		EventBusMount(const EventBusMount<EventIdT>& other) = delete;
		EventBusMount& operator=(const EventBusMount<EventIdT>& other) = delete;
		EventBusMount(EventBusMount<EventIdT>&& other) = delete;
		EventBusMount& operator=(EventBusMount<EventIdT>&& other) = delete;
		~EventBusMount();

		GenericEventBus<EventIdT>* GetEventBus() const;
		const std::vector<EventBusMount<EventIdT>*>& GetParents() const;
		const std::vector<EventBusMount<EventIdT>*>& GetChildren() const;

		void AddParent(EventBusMount<EventIdT>* bus);
		void AddChild(EventBusMount<EventIdT>* bus);
		void RemoveParent(EventBusMount<EventIdT>* bus);
		void RemoveChild(EventBusMount<EventIdT>* bus);
		void SetEventBus(GenericEventBus<EventIdT>* bus);

	};

	// ============================================================================================================================================================================
	// EVENT BUS IMPLEMENTATION
	// ============================================================================================================================================================================

	template<typename EventIdT>
	GenericEventBus<EventIdT>::GenericEventBus(bool addToEventManager)
		: m_ListenersMutex(), m_EventsMutex(), m_Listeners(), m_ListenerLocations(), m_Events(), m_ListenerIds(0, (uint32_t)-1), m_MountManager(std::make_unique<EventBusMount<EventIdT>>(this))
	{
		if (addToEventManager)
		{
			EventManager::Get().AddEventBus(this);
		}		
	}

	template<typename EventIdT>
	GenericEventBus<EventIdT>::GenericEventBus(GenericEventBus<EventIdT>&& other)
		: m_ListenersMutex(), m_EventsMutex(), m_Listeners(std::move(other.m_Listeners)), m_ListenerLocations(std::move(other.m_ListenerLocations)), m_Events(std::move(other.m_Events)),
		m_ListenerIds(std::move(other.m_ListenerIds)), m_MountManager(std::move(other.m_MountManager))
	{
		m_MountManager->SetEventBus(this);
		EventManager::Get().UpdateEventBus(&other, this);
	}

	template<typename EventIdT>
	GenericEventBus<EventIdT>& GenericEventBus<EventIdT>::operator=(GenericEventBus<EventIdT>&& other)
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

	template<typename EventIdT>
	GenericEventBus<EventIdT>::~GenericEventBus()
	{
		EventManager::Get().RemoveBus(this);
	}

	template<typename EventIdT>
	template<typename T>
	GenericEventEmitter<T, EventIdT> GenericEventBus<EventIdT>::GetEmitter(const EventIdT& eventId)
	{
		return GenericEventEmitter<T, EventIdT>(eventId, *this);
	}

	template<typename EventIdT>
	EventBusMount<EventIdT>* GenericEventBus<EventIdT>::GetMount() const
	{
		return m_MountManager.get();
	}

	template<typename EventIdT>
	void GenericEventBus<EventIdT>::MountOn(GenericEventBus<EventIdT>& bus) const
	{
		BLT_ASSERT(&bus != this, "Cannot mount on self");
		GetMount()->AddParent(bus.GetMount());
		bus.GetMount()->AddChild(GetMount());
	}

	template<typename EventIdT>
	template<typename T>
	uint32_t GenericEventBus<EventIdT>::On(const EventIdT& eventId, const typename EventListener<T>::callback_t& callback)
	{
		std::scoped_lock<std::mutex> lock(m_ListenersMutex);
		uint32_t id = m_ListenerIds.GetNextId();
		std::unique_ptr<EventListener<T>> listener = std::make_unique<EventListener<T>>(callback);
		m_ListenerLocations[id] = { eventId, listener.get() };
		m_Listeners[eventId].push_back(std::move(listener));
		return id;
	}

	template<typename EventIdT>
	void GenericEventBus<EventIdT>::RemoveEventListener(uint32_t id)
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

	template<typename EventIdT>
	template<typename T>
	void GenericEventBus<EventIdT>::Emit(const EventIdT& eventId, const T& data)
	{
		EmitEvent(eventId, std::make_unique<Event<T>>(data));
	}

	template<typename EventIdT>
	template<typename T>
	void GenericEventBus<EventIdT>::Emit(const EventIdT& eventId, T&& data)
	{
		EmitEvent(eventId, std::make_unique<Event<T>>(std::move(data)));
	}

	template<typename EventIdT>
	void GenericEventBus<EventIdT>::Emit(const EventIdT& eventId)
	{
		EmitEvent(eventId, std::make_unique<Event<void>>());
	}

	template<typename EventIdT>
	void GenericEventBus<EventIdT>::Flush()
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
		}
	}

	template<typename EventIdT>
	void GenericEventBus<EventIdT>::EmitEvent(const EventIdT& eventId, std::unique_ptr<EventContainer>&& event)
	{
		std::scoped_lock<std::mutex> lock(m_EventsMutex);
		m_Events.push_back({ eventId, std::move(event) });
	}

	template<typename EventIdT>
	void GenericEventBus<EventIdT>::ProcessEvent(EventInfo& e) const
	{
		if (!e.Event->Handled)
		{
			if (m_Listeners.find(e.Id) != m_Listeners.end())
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
			if (!e.Event->Handled)
			{
				for (EventBusMount<EventIdT>* mount : GetMount()->GetChildren())
				{
					mount->GetEventBus()->ProcessEvent(e);
					if (e.Event->Handled)
					{
						break;
					}
				}
			}
		}
	}

	using EventBus = GenericEventBus<uint32_t>;

	// ============================================================================================================================================================================
	// EVENT BUS MOUNT IMPLEMENTATION
	// ============================================================================================================================================================================

	template<typename EventIdT>
	EventBusMount<EventIdT>::EventBusMount(GenericEventBus<EventIdT>* bus)
		: m_EventBus(bus), m_Parents(), m_Children()
	{

	}

	template<typename EventIdT>
	EventBusMount<EventIdT>::~EventBusMount()
	{
		for (EventBusMount<EventIdT>* parent : m_Parents)
		{
			parent->RemoveChild(this);
		}
		for (EventBusMount<EventIdT>* child : m_Children)
		{
			child->RemoveParent(this);
		}
	}

	template<typename EventIdT>
	GenericEventBus<EventIdT>* EventBusMount<EventIdT>::GetEventBus() const
	{
		return m_EventBus;
	}

	template<typename EventIdT>
	const std::vector<EventBusMount<EventIdT>*>& EventBusMount<EventIdT>::GetParents() const
	{
		return m_Parents;
	}

	template<typename EventIdT>
	const std::vector<EventBusMount<EventIdT>*>& EventBusMount<EventIdT>::GetChildren() const
	{
		return m_Children;
	}

	template<typename EventIdT>
	void EventBusMount<EventIdT>::AddParent(EventBusMount<EventIdT>* bus)
	{
		m_Parents.push_back(bus);
	}

	template<typename EventIdT>
	void EventBusMount<EventIdT>::AddChild(EventBusMount<EventIdT>* bus)
	{
		m_Children.push_back(bus);
	}

	template<typename EventIdT>
	void EventBusMount<EventIdT>::RemoveParent(EventBusMount<EventIdT>* bus)
	{
		auto it = std::find(m_Parents.begin(), m_Parents.end(), bus);
		if (it != m_Parents.end())
		{
			m_Parents.erase(it);
		}
	}

	template<typename EventIdT>
	void EventBusMount<EventIdT>::RemoveChild(EventBusMount<EventIdT>* bus)
	{
		auto it = std::find(m_Children.begin(), m_Children.end(), bus);
		if (it != m_Children.end())
		{
			m_Children.erase(it);
		}
	}

	template<typename EventIdT>
	void EventBusMount<EventIdT>::SetEventBus(GenericEventBus<EventIdT>* bus)
	{
		m_EventBus = bus;
	}

}