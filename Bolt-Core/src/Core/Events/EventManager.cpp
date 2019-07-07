#include "bltpch.h"
#include "EventManager.h"
#include "EventBus.h"

#include "Engine/Engine.h"

namespace Bolt
{

	EventManager& EventManager::Get()
	{
		return Engine::Instance().CurrentContext().GetRenderContext().GetEventManager();
	}

	EventManager::EventManager()
		: m_EventBuses(), m_GlobalBus(std::make_unique<GenericEventBus<uint32_t>>())
	{
		m_GlobalBus->On<TaskCompletedEvent>(Events::TASK_CONTINUE_ON_MAIN_THREAD, [](Event<TaskCompletedEvent>& e)
			{
				e.Data.Execute();
			});
	}

	EventBus& EventManager::Bus()
	{
		return *m_GlobalBus;
	}

	void EventManager::AddEventBus(EventBusBase* bus)
	{
		m_EventBuses.push_back(bus);
	}

	void EventManager::UpdateEventBus(EventBusBase* oldBus, EventBusBase* newBus)
	{
		auto it = std::find(m_EventBuses.begin(), m_EventBuses.end(), oldBus);
		if (it != m_EventBuses.end())
		{
			*it = newBus;
		}
		else
		{
			BLT_CORE_WARN("BUS NOT FOUND");
		}
	}

	void EventManager::RemoveBus(EventBusBase* bus)
	{
		auto it = std::find(m_EventBuses.begin(), m_EventBuses.end(), bus);
		if (it != m_EventBuses.end())
		{
			m_EventBuses.erase(it);
		}
		else
		{
			BLT_CORE_WARN("BUS NOT FOUND");
		}
	}

	void EventManager::FlushAll() const
	{
		for (int i = m_EventBuses.size() - 1; i >= 0; i--)
		{
			m_EventBuses[i]->Flush();
		}
	}

}