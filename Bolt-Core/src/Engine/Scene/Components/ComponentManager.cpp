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

	Component& ComponentManager::GetComponentById(id_t id) const
	{
		return *m_ComponentArray[id];
	}

	Component& ComponentManager::GetComponent(size_t componentTypeHash) const
	{
		BLT_ASSERT(HasComponent(componentTypeHash), "Does not have given component type");
		return *m_ComponentMap.at(componentTypeHash);
	}

	bool ComponentManager::HasComponentById(id_t id) const
	{
		return m_ComponentArray[id] != nullptr;
	}

	bool ComponentManager::HasComponent(size_t componentTypeHash) const
	{
		if (m_ComponentMap.empty())
		{
			return false;
		}
		return m_ComponentMap.find(componentTypeHash) != m_ComponentMap.end();
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

	Component* ComponentManager::AddComponent(size_t componentTypeHash, std::unique_ptr<Component>&& component)
	{
		id_t id = FindNextId();
		component->m_Id = id;
		m_ComponentMap[componentTypeHash] = std::move(component);
		Component* c = m_ComponentMap.at(componentTypeHash).get();
		m_ComponentArray[id] = c;
		c->SetGameObject(m_GameObject);
		c->Start();
		return c;
	}

	void ComponentManager::RemoveComponentById(id_t id)
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

	void ComponentManager::RemoveComponent(size_t componentTypeHash)
	{
		Component& component = GetComponent(componentTypeHash);
		m_ComponentArray[component.m_Id] = nullptr;
		m_ComponentMap.erase(componentTypeHash);
	}

	void ComponentManager::Clear()
	{
		m_ComponentMap.clear();
	}

	void ComponentManager::Transfer(XMLserializer& backend, bool isWriting)
	{
		BLT_TRANSFER(backend, m_ComponentMap);
		BLT_TRANSFER(backend, m_GameObject);
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