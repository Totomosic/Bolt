#pragma once
#include "EventData.h"
#include "EventManager.h"
#include "EventEmitter.h"
#include "IdManager.h"

#include "BoltLib/Profiling/Profiling.h"

namespace Bolt
{

	// ============================================================================================================================================================================
	// EVENT BUS DECLARATION
	// ============================================================================================================================================================================

	template<typename T>
	class EventBusMount;

	template<typename T>
	class GenericScopedEventListener;

	class BLT_API EventBusBase
	{
	public:
		virtual ~EventBusBase() {}
		virtual void RemoveEventListener(uint32_t listenerId) = 0;
		virtual void Flush() = 0;
	};

	template<typename EventIdT>
	class BLT_API GenericEventBus : public EventBusBase
	{
	public:
		using EventId = EventIdT;
		template<typename T>
		using emitter = GenericEventEmitter<T, EventIdT>;
		using scoped_listener = GenericScopedEventListener<EventIdT>;

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
		mutable EventBusMount<EventIdT> m_MountManager;
		std::vector<EventEmitterBase*> m_EventEmitters;

		bool m_ImmediateMode;

	public:
		GenericEventBus(bool addToEventManager = true);
		GenericEventBus(const GenericEventBus<EventIdT>& other) = delete;
		GenericEventBus<EventIdT>& operator=(const GenericEventBus<EventIdT>& other) = delete;
		GenericEventBus(GenericEventBus<EventIdT>&& other) = delete;
		GenericEventBus<EventIdT>& operator=(GenericEventBus<EventIdT>&& other) = delete;
		~GenericEventBus();

		bool IsImmediateMode() const;
		void SetImmediateMode(bool immediate);

		template<typename T>
		GenericEventEmitter<T, EventIdT> GetEmitter(const EventIdT& eventId);
		EventBusMount<EventIdT>* GetMount() const;

		void MountOn(GenericEventBus<EventIdT>& bus) const;
		template<typename T>
		uint32_t AddEventListener(const EventIdT& eventId, const typename EventListener<T>::callback_t& callback, ListenerPriority priority = ListenerPriority::Medium);
		template<typename T>
		GenericScopedEventListener<EventIdT> AddScopedEventListener(const EventIdT& eventId, const typename EventListener<T>::callback_t& callback, ListenerPriority priority = ListenerPriority::Medium);

		int GetListenerPriorityIndex(uint32_t id) const;
		void SetListenerPriorityIndex(uint32_t id, int priorityIndex);
		void RemoveEventListener(uint32_t id) override;

		template<typename T>
		void Emit(const EventIdT& eventId, const T& data);
		template<typename T>
		void Emit(const EventIdT& eventId, T&& data);
		void Emit(const EventIdT& eventId);
		virtual void Flush() override;

	private:
		void EmitEvent(const EventIdT& eventId, std::unique_ptr<EventContainer>&& event);
		void ProcessEvent(EventInfo& e) const;

		static void PushEventListener(std::vector<std::unique_ptr<EventListenerContainer>>& vector, std::unique_ptr<EventListenerContainer>&& listener);
		static void PushEventListenerHigh(std::vector<std::unique_ptr<EventListenerContainer>>& vector, std::unique_ptr<EventListenerContainer>&& listener);
		static void PushEventListenerMedium(std::vector<std::unique_ptr<EventListenerContainer>>& vector, std::unique_ptr<EventListenerContainer>&& listener);
		static void PushEventListenerLow(std::vector<std::unique_ptr<EventListenerContainer>>& vector, std::unique_ptr<EventListenerContainer>&& listener);

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
	// SCOPED EVENT LISTENER DECLARATION
	// ============================================================================================================================================================================

	template<typename EventIdT>
	class BLT_API GenericScopedEventListener
	{
	private:
		GenericEventBus<EventIdT>* m_EventBus;
		uint32_t m_ListenerId;

	public:
		GenericScopedEventListener();
		GenericScopedEventListener(GenericEventBus<EventIdT>& eventBus, uint32_t listenerId);
		GenericScopedEventListener(const GenericScopedEventListener<EventIdT>& other) = delete;
		GenericScopedEventListener<EventIdT>& operator=(const GenericScopedEventListener<EventIdT>& other) = delete;
		GenericScopedEventListener(GenericScopedEventListener<EventIdT>&& other);
		GenericScopedEventListener<EventIdT>& operator=(GenericScopedEventListener<EventIdT>&& other);
		~GenericScopedEventListener();

		GenericEventBus<EventIdT>& GetEventBus() const;
		uint32_t GetListenerId() const;
		uint32_t ReleaseListener();

	};

	// ============================================================================================================================================================================
	// EVENT BUS IMPLEMENTATION
	// ============================================================================================================================================================================

	template<typename EventIdT>
	GenericEventBus<EventIdT>::GenericEventBus(bool addToEventManager)
		: m_ListenersMutex(), m_EventsMutex(), m_Listeners(), m_ListenerLocations(), m_Events(), m_ListenerIds(0, (uint32_t)-1), m_MountManager(this), m_ImmediateMode(false)
	{
		if (addToEventManager)
		{
			EventManager::Get().AddEventBus(this);
		}
	}

	template<typename EventIdT>
	GenericEventBus<EventIdT>::~GenericEventBus()
	{
		EventManager::Get().RemoveBus(this);
	}

	template<typename EventIdT>
	inline bool GenericEventBus<EventIdT>::IsImmediateMode() const
	{
		return m_ImmediateMode;
	}

	template<typename EventIdT>
	void GenericEventBus<EventIdT>::SetImmediateMode(bool immediate)
	{
		m_ImmediateMode = immediate;
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
		return &m_MountManager;
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
	uint32_t GenericEventBus<EventIdT>::AddEventListener(const EventIdT& eventId, const typename EventListener<T>::callback_t& callback, ListenerPriority priority)
	{
		BLT_PROFILE_FUNCTION();
		BLT_ASSERT(priority != ListenerPriority::Custom, "Cannot create a listener with custom priority");
		std::scoped_lock<std::mutex> lock(m_ListenersMutex);
		uint32_t id = m_ListenerIds.GetNextId();
		std::unique_ptr<EventListener<T>> listener = std::make_unique<EventListener<T>>(callback, priority);
		m_ListenerLocations[id] = { eventId, listener.get() };
		PushEventListener(m_Listeners[eventId], std::move(listener));
		return id;
	}

	template<typename EventIdT>
	template<typename T>
	GenericScopedEventListener<EventIdT> GenericEventBus<EventIdT>::AddScopedEventListener(const EventIdT& eventId, const typename EventListener<T>::callback_t& callback, ListenerPriority priority)
	{
		return GenericScopedEventListener<EventIdT>(*this, AddEventListener<T>(eventId, callback, priority));
	}

	template<typename EventIdT>
	int GenericEventBus<EventIdT>::GetListenerPriorityIndex(uint32_t id) const
	{
		BLT_PROFILE_FUNCTION();
		std::scoped_lock<std::mutex> lock(m_ListenersMutex);
		ListenerLocation& location = m_ListenerLocations.at(id);
		std::vector<std::unique_ptr<EventListenerContainer>>& vector = m_Listeners[location.Id];
		for (int i = 0; i < vector.size(); i++)
		{
			if (vector.at(i).get() == location.Listener)
			{
				return i;
			}
		}
		BLT_ASSERT(false, "No listener with id {}", id);
		return -1;
	}

	template<typename EventIdT>
	void GenericEventBus<EventIdT>::SetListenerPriorityIndex(uint32_t id, int priorityIndex)
	{
		BLT_PROFILE_FUNCTION();
		std::scoped_lock<std::mutex> lock(m_ListenersMutex);
		ListenerLocation& location = m_ListenerLocations.at(id);
		std::vector<std::unique_ptr<EventListenerContainer>>& vector = m_Listeners[location.Id];
		int index = -1;
		for (int i = 0; i < vector.size(); i++)
		{
			if (vector.at(i).get() == location.Listener)
			{
				index = i;
				break;
			}
		}
		BLT_ASSERT(index != -1, "No listener with id {}", id);
		if (priorityIndex >= vector.size())
		{
			priorityIndex = (int)vector.size() - 1;
		}
		vector.at(index)->m_Priority = ListenerPriority::Custom;
		std::rotate(vector.begin() + priorityIndex, vector.begin() + index, vector.begin() + index + 1);
	}

	template<typename EventIdT>
	void GenericEventBus<EventIdT>::RemoveEventListener(uint32_t id)
	{
		BLT_PROFILE_FUNCTION();
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
		BLT_PROFILE_FUNCTION();
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
		BLT_PROFILE_FUNCTION();
		if (m_ImmediateMode)
		{
			GenericEventBus<EventIdT>::EventInfo e{ eventId, std::move(event) };
			ProcessEvent(e);
		}
		else
		{
			std::scoped_lock<std::mutex> lock(m_EventsMutex);
			m_Events.push_back({ eventId, std::move(event) });
		}
	}

	template<typename EventIdT>
	void GenericEventBus<EventIdT>::ProcessEvent(EventInfo& e) const
	{
		BLT_PROFILE_FUNCTION();
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

	template<typename EventIdT>
	void GenericEventBus<EventIdT>::PushEventListener(std::vector<std::unique_ptr<EventListenerContainer>>& vector, std::unique_ptr<EventListenerContainer>&& listener)
	{
		switch (listener->GetPriority())
		{
		case ListenerPriority::Low:
			PushEventListenerLow(vector, std::move(listener));
			break;
		case ListenerPriority::Medium:
			PushEventListenerMedium(vector, std::move(listener));
			break;
		case ListenerPriority::High:
			PushEventListenerHigh(vector, std::move(listener));
			break;
		default:
			break;
		}
	}

	template<typename EventIdT>
	void GenericEventBus<EventIdT>::PushEventListenerHigh(std::vector<std::unique_ptr<EventListenerContainer>>& vector, std::unique_ptr<EventListenerContainer>&& listener)
	{
		if (vector.size() == 0)
		{
			vector.push_back(std::move(listener));
			return;
		}
		for (int i = 0; i < vector.size(); i++)
		{
			EventListenerContainer& container = *vector.at(i);
			if (container.GetPriority() < ListenerPriority::High)
			{
				vector.insert(vector.begin() + i, std::move(listener));
				return;
			}
		}
		vector.push_back(std::move(listener));
	}

	template<typename EventIdT>
	void GenericEventBus<EventIdT>::PushEventListenerMedium(std::vector<std::unique_ptr<EventListenerContainer>>& vector, std::unique_ptr<EventListenerContainer>&& listener)
	{
		if (vector.size() == 0)
		{
			vector.push_back(std::move(listener));
			return;
		}
		for (int i = 0; i < vector.size(); i++)
		{
			EventListenerContainer& container = *vector.at(i);
			if (container.GetPriority() < ListenerPriority::Medium)
			{
				vector.insert(vector.begin() + i, std::move(listener));
				return;
			}
		}
		vector.push_back(std::move(listener));
	}

	template<typename EventIdT>
	void GenericEventBus<EventIdT>::PushEventListenerLow(std::vector<std::unique_ptr<EventListenerContainer>>& vector, std::unique_ptr<EventListenerContainer>&& listener)
	{
		if (vector.size() == 0)
		{
			vector.push_back(std::move(listener));
			return;
		}
		for (int i = 0; i < vector.size(); i++)
		{
			EventListenerContainer& container = *vector.at(i);
			if (container.GetPriority() == ListenerPriority::Low)
			{
				vector.insert(vector.begin() + i, std::move(listener));
				return;
			}
		}
		vector.push_back(std::move(listener));
	}

	using EventBus = GenericEventBus<uint32_t>;

	// ============================================================================================================================================================================
	// SCOPED EVENT LISTENER IMPLEMENTATION
	// ============================================================================================================================================================================

	template<typename EventIdT>
	GenericScopedEventListener<EventIdT>::GenericScopedEventListener()
		: m_EventBus(nullptr), m_ListenerId(0)
	{

	}

	template<typename EventIdT>
	GenericScopedEventListener<EventIdT>::GenericScopedEventListener(GenericEventBus<EventIdT>& eventBus, uint32_t listenerId)
		: m_EventBus(&eventBus), m_ListenerId(listenerId)
	{

	}

	template<typename EventIdT>
	GenericScopedEventListener<EventIdT>::GenericScopedEventListener(GenericScopedEventListener<EventIdT>&& other)
		: m_EventBus(other.m_EventBus), m_ListenerId(other.m_ListenerId)
	{
		other.m_EventBus = nullptr;
	}

	template<typename EventIdT>
	GenericScopedEventListener<EventIdT>& GenericScopedEventListener<EventIdT>::operator=(GenericScopedEventListener<EventIdT>&& other)
	{
		GenericEventBus<EventIdT>* bus = m_EventBus;
		m_EventBus = other.m_EventBus;
		m_ListenerId = other.m_ListenerId;
		other.m_EventBus = bus;
		return *this;
	}

	template<typename EventIdT>
	GenericScopedEventListener<EventIdT>::~GenericScopedEventListener()
	{
		if (m_EventBus != nullptr)
		{
			m_EventBus->RemoveEventListener(m_ListenerId);
		}
	}

	template<typename EventIdT>
	GenericEventBus<EventIdT>& GenericScopedEventListener<EventIdT>::GetEventBus() const
	{
		return *m_EventBus;
	}

	template<typename EventIdT>
	uint32_t GenericScopedEventListener<EventIdT>::GetListenerId() const
	{
		return m_ListenerId;
	}

	template<typename EventIdT>
	uint32_t GenericScopedEventListener<EventIdT>::ReleaseListener()
	{
		m_EventBus = nullptr;
		return m_ListenerId;
	}

	using ScopedEventListener = typename EventBus::scoped_listener;

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