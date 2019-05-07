#include "Types.h"
#include "PhysicsManager.h"
#include "../Scene.h"

namespace Bolt
{

	PhysicsManager::PhysicsManager(Scene* scene)
		: m_Scene(scene), m_Engines(), m_LayerMaskMap()
	{
		ResetMaskMap();
	}

	const PhysicsEngine& PhysicsManager::GetEngineById(id_t id) const
	{
		return m_Engines[id].Engine;
	}

	PhysicsEngine& PhysicsManager::GetEngineById(id_t id)
	{
		return m_Engines[id].Engine;
	}

	id_t PhysicsManager::GetIdOfEngine(id_t layerId) const
	{
		id_t layerMask = m_Scene->GetMaskOfLayer(layerId);
		if (m_LayerMaskMap.find(layerMask) != m_LayerMaskMap.end())
		{
			return m_LayerMaskMap.at(layerMask);
		}
		auto begin = m_LayerMaskMap.begin();
		auto end = m_LayerMaskMap.end();
		for (; begin != end; begin++)
		{
			id_t mask = (*begin).first;
			if ((mask & layerMask) != 0)
			{
				return (*begin).second;
			}
		}
		BLT_ASSERT(false, "Unable to find layer");
		return -1;
	}

	id_t PhysicsManager::MergeLayers(id_t layerMask)
	{
		// Ensure that no other merge exists with these layers
		BLT_ASSERT(std::find_if(m_LayerMaskMap.begin(), m_LayerMaskMap.end(), [layerMask](const auto& pair)
		{
			return ((pair.first & layerMask) != 0) && ((pair.first & layerMask) != pair.first);
		}) == m_LayerMaskMap.end(), "Unable to merge layers");
		std::vector<GameObject*> objects;
		std::vector<id_t> removeMasks;
		auto begin = m_LayerMaskMap.begin();
		auto end = m_LayerMaskMap.end();
		id_t selectedEngine = (id_t)-1;
		for (; begin != end; begin++)
		{
			id_t mask = (*begin).first;
			if ((mask & layerMask) != 0)
			{
				EngineInfo& engine = m_Engines[m_LayerMaskMap.at(mask)];
				if (selectedEngine == (id_t)-1)
				{
					selectedEngine = m_LayerMaskMap.at(mask);
					engine.Engine.Clear();
				}
				else
				{
					engine.Engine.Clear();
					engine.InUse = false;
					removeMasks.push_back(mask);
				}
				for (GameObject* object : engine.Engine.Objects())
				{
					objects.push_back(object);
				}
			}
		}
		if (selectedEngine != (id_t)-1)
		{
			for (GameObject* object : objects)
			{
				m_Engines[selectedEngine].Engine.AddGameObject(object);
			}
			for (id_t mask : removeMasks)
			{
				m_LayerMaskMap.erase(mask);
			}
			m_LayerMaskMap[layerMask] = selectedEngine;
			return selectedEngine;
		}
		BLT_ASSERT(false, "Error occurred");
		return (id_t)-1;
	}

	id_t PhysicsManager::AddGameObject(GameObject* object)
	{
		id_t engineId = GetIdOfEngine(object->GetLayer()->Id());
		GetEngineById(engineId).AddGameObject(object);
		return engineId;
	}

	id_t PhysicsManager::RemoveGameObject(GameObject* object)
	{
		id_t engineId = GetIdOfEngine(object->GetLayer()->Id());
		GetEngineById(engineId).RemoveGameObject(object);
		return engineId;
	}

	void PhysicsManager::ResetMaskMap()
	{
		m_LayerMaskMap.clear();
		for (id_t i = 0; i < MAX_PHYSICS_ENGINES; i++)
		{
			m_LayerMaskMap[m_Scene->GetMaskOfLayer(i)] = i;
			m_Engines->Engine = PhysicsEngine();
			m_Engines->InUse = true;
		}
	}

}