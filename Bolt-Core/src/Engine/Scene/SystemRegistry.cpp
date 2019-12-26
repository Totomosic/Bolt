#include "bltpch.h"
#include "SystemRegistry.h"

namespace Bolt
{

	SystemRegistry::SystemRegistry(EntityManager& manager)
		: m_Manager(manager), m_Systems(), m_SystemOrder()
	{

	}

	int SystemRegistry::GetSystemCount() const
	{
		return m_SystemOrder.size();
	}

	void SystemRegistry::Configure() const
	{
		for (const auto& pair : m_Systems)
		{
			pair.second->Configure();
		}
	}

	void SystemRegistry::Update(const SystemPtr<BaseSystem>& system, TimeDelta delta) const
	{
		UpdateSystem(*system, delta);
	}

	void SystemRegistry::UpdateAll(TimeDelta delta) const
	{
		for (BaseSystem::Family family : m_SystemOrder)
		{
			UpdateSystem(*m_Systems.at(family), delta);
		}
	}

	void SystemRegistry::UpdateSystem(BaseSystem& system, TimeDelta delta) const
	{
		system.Update(m_Manager, delta);
	}

}