#pragma once
#include "Events.h"

namespace Bolt
{

	template<typename> class GenericEventBus;

	template<typename T, typename EventIdT>
	class BLT_API GenericEventEmitter
	{
	private:
		EventIdT m_EventId;
		GenericEventBus<EventIdT>& m_EventBus;

	public:
		GenericEventEmitter(const EventIdT& eventId, GenericEventBus<EventIdT>& bus)
			: m_EventId(eventId), m_EventBus(bus)
		{
		
		}

		const EventIdT& EventId() const
		{
			return m_EventId;
		}

		GenericEventBus<EventIdT>& Bus()
		{
			return m_EventBus;
		}

		uint32_t On(const typename EventListener<T>::callback_t& callback)
		{
			return m_EventBus.On<T>(m_EventId, callback);
		}

		void Emit(const T& data)
		{
			m_EventBus.Emit(m_EventId, data);
		}

		void Emit(T&& data)
		{
			m_EventBus.Emit(m_EventId, std::move(data));
		}

		void Emit()
		{
			m_EventBus.Emit(m_EventId);
		}

	};

	template<typename T>
	using EventEmitter = GenericEventEmitter<T, uint32_t>;

}