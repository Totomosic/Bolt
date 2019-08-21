#pragma once
#include "Events.h"

namespace Bolt
{

	template<typename> class GenericEventBus;
	template<typename> class GenericScopedEventListener;

	class BLT_API EventEmitterBase
	{
	public:
		virtual ~EventEmitterBase() {}
		virtual void RemoveEventListener(uint32_t listenerId) = 0;
		
		template<typename> friend class GenericEventBus;

	protected:
		virtual void SetEventBus(EventBusBase* eventBus) = 0;
	};

	template<typename T, typename EventIdT>
	class BLT_API GenericEventEmitter : public EventEmitterBase
	{
	public:
		using callback_t = typename EventListener<T>::callback_t;

	private:
		EventIdT m_EventId;
		GenericEventBus<EventIdT>* m_EventBus;

	public:
		GenericEventEmitter(const EventIdT& eventId, GenericEventBus<EventIdT>& bus)
			: m_EventId(eventId), m_EventBus(&bus)
		{
			m_EventBus->AddEventEmitter(this);
		}

		GenericEventEmitter(GenericEventEmitter<T, EventIdT>&& other)
			: m_EventId(other.m_EventId), m_EventBus(other.m_EventBus)
		{
			m_EventBus->UpdateEventEmitter(&other, this);
			other.m_EventBus = nullptr;
		}

		GenericEventEmitter<T, EventIdT>& operator=(GenericEventEmitter<T, EventIdT>&& other)
		{
			m_EventId = other.m_EventId;
			GenericEventBus<EventIdT>* bus = m_EventBus;
			m_EventBus = other.m_EventBus;
			other.m_EventBus = bus;
			m_EventBus->UpdateEventEmitter(&other, this);
			return *this;
		}

		~GenericEventEmitter() override
		{
			if (m_EventBus != nullptr)
			{
				m_EventBus->RemoveEventEmitter(this);
			}
		}

		const EventIdT& EventId() const
		{
			return m_EventId;
		}

		GenericEventBus<EventIdT>& Bus()
		{
			return *m_EventBus;
		}

		uint32_t AddEventListener(const typename callback_t& callback, ListenerPriority priority = ListenerPriority::Medium)
		{
			return m_EventBus->AddEventListener<T>(m_EventId, callback, priority);
		}

		GenericScopedEventListener<EventIdT> AddScopedEventListener(const typename callback_t& callback, ListenerPriority priority = ListenerPriority::Medium)
		{
			return m_EventBus->AddScopedEventListener<T>(m_EventId, callback, priority);
		}

		void RemoveEventListener(uint32_t listenerId) override
		{
			m_EventBus->RemoveEventListener(listenerId);
		}

		void Emit(const T& data)
		{
			m_EventBus->Emit(m_EventId, data);
		}

		void Emit(T&& data)
		{
			m_EventBus->Emit(m_EventId, std::move(data));
		}

		void Emit()
		{
			m_EventBus->Emit(m_EventId);
		}

	protected:
		void SetEventBus(EventBusBase* eventBus) override
		{
			m_EventBus = (GenericEventBus<EventIdT>*)eventBus;
		}

	};

	template<typename T>
	using EventEmitter = GenericEventEmitter<T, uint32_t>;

}