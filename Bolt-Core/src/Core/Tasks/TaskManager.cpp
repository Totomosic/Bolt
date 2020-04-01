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
		: m_Bus(bus), m_Pool(), m_Listener()
	{
		m_Listener = m_Bus.AddScopedEventListener<TaskCompletedEvent>(TaskBase::TaskEventId, [](Event<TaskCompletedEvent>& e)
			{
				e.Data.Execute();
				e.StopPropagation();
			});
		m_Pool.Start();
	}

	EventBus& TaskManager::Bus() const
	{
		return m_Bus;
	}

	ThreadPool& TaskManager::GetThreadPool()
	{
		return m_Pool;
	}

}