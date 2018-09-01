#include "ComponentManager.h"
#include "..\GameObject.h"

namespace Bolt
{

	ComponentManager::ComponentManager(ObjectPrefab* gameObject)
		: m_ComponentMap(), m_ComponentArray(), m_GameObject(gameObject)
	{
		for (int i = 0; i < ComponentManager::MAX_COMPONENTS; i++)
		{
			m_ComponentArray[i] = nullptr;
		}
	}

	ComponentManager::ComponentManager(ComponentManager&& other)
		: m_ComponentMap(std::move(other.m_ComponentMap)), m_GameObject(other.m_GameObject)
	{
		for (int i = 0; i < ComponentManager::MAX_COMPONENTS; i++)
		{
			m_ComponentArray[i] = nullptr;
		}
		for (auto& pair : m_ComponentMap)
		{
			m_ComponentArray[pair.second->m_Id] = pair.second.get();
		}
	}

	ComponentManager& ComponentManager::operator=(ComponentManager&& other)
	{
		m_ComponentMap = std::move(other.m_ComponentMap);
		m_GameObject = other.m_GameObject;
		for (int i = 0; i < ComponentManager::MAX_COMPONENTS; i++)
		{
			m_ComponentArray[i] = nullptr;
		}
		for (auto& pair : m_ComponentMap)
		{
			m_ComponentArray[pair.second->m_Id] = pair.second.get();
		}
		return *this;
	}

	ComponentManager::~ComponentManager()
	{
	
	}

	Component& ComponentManager::GetComponent(id_t id) const
	{
		return *m_ComponentArray[id];
	}

	Component& ComponentManager::GetComponent(const std::type_index& componentType) const
	{
		return *m_ComponentMap.at(componentType);
	}

	bool ComponentManager::HasComponent(id_t id) const
	{
		return m_ComponentArray[id] != nullptr;
	}

	bool ComponentManager::HasComponent(const std::type_index& componentType) const
	{
		if (m_ComponentMap.empty())
		{
			return false;
		}
		return m_ComponentMap.find(componentType) != m_ComponentMap.end();
	}

	std::vector<Component*> ComponentManager::GetComponents() const
	{
		std::vector<Component*> result;
		for (const auto& pair : m_ComponentMap)
		{
			result.push_back(pair.second.get());
		}
		return result;
	}

	Component* ComponentManager::AddComponent(const std::type_index& componentType, std::unique_ptr<Component>&& component)
	{
		id_t id = FindNextId();
		component->m_Id = id;
		m_ComponentMap[componentType] = std::move(component);
		Component* c = m_ComponentMap.at(componentType).get();
		m_ComponentArray[id] = c;
		c->SetGameObject(m_GameObject);
		c->Start();
		return c;
	}

	void ComponentManager::RemoveComponent(id_t id)
	{
		Component* component = m_ComponentArray[id];
		component->End();
		for (auto& pair : m_ComponentMap)
		{
			if (pair.second.get() == component)
			{
				m_ComponentMap.erase(pair.first);
				break;
			}
		}
		m_ComponentArray[id] = nullptr;
	}

	void ComponentManager::RemoveComponent(const std::type_index& componentType)
	{
		Component& component = GetComponent(componentType);
		m_ComponentArray[component.m_Id] = nullptr;
		m_ComponentMap.erase(componentType);
	}

	id_t ComponentManager::FindNextId() const
	{
		for (id_t i = 0; i < ComponentManager::MAX_COMPONENTS; i++)
		{
			if (m_ComponentArray[i] == nullptr)
			{
				return i;
			}
		}
		return GameObject::InvalidID;
	}

	void ComponentManager::SetGameObject(ObjectPrefab* object)
	{
		m_GameObject = object;
		for (auto& pair : m_ComponentMap)
		{
			pair.second->SetGameObject(object);
		}
	}

}