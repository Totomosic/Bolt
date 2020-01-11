#include "bltpch.h"
#include "TaskManager.h"

#include "Engine/Engine.h"

namespace Bolt
{

	TaskManager& TaskManager::Get()
	{
		return Engine::Instance().CurrentContext().GetTaskManager();
	}

	TaskManager::TaskManager(EventBus& bus)
		: m_Bus(bus), m_Listener()
	{
		m_Listener = m_Bus.AddScopedEventListener<TaskCompletedEvent>(Events::Internal.AsyncTaskCompleted, [](Event<TaskCompletedEvent>& e)
			{
				e.Data.Execute();
				e.StopPropagation();
			});
	}

	EventBus& TaskManager::Bus() const
	{
		return m_Bus;
	}

}