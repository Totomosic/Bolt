#pragma once
#include "bltpch.h"

namespace Bolt
{

	template<typename> class GenericEventBus;
	class EventBusBase;

	class BLT_API EventManager
	{
	private:
		std::vector<EventBusBase*> m_EventBuses;
		std::unique_ptr<GenericEventBus<uint32_t>> m_GlobalBus;

	public:
		static EventManager& Get();

	public:
		EventManager();

		GenericEventBus<uint32_t>& Bus();

		void AddEventBus(EventBusBase* bus);
		void AddEventBus(GenericEventBus<uint32_t>* bus);
		void UpdateEventBus(EventBusBase* oldBus, EventBusBase* newBus);
		void RemoveBus(EventBusBase* bus);
		void FlushAll() const;

	};

}