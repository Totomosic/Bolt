#include "bltpch.h"
#include "ComponentManager.h"
#include "../GameObject.h"

#include "Core/Profiling/Profiling.h"

namespace Bolt
{

	ComponentManager::ComponentManager(ObjectPrefab* gameObject)
		: m_ComponentIdManager(0, MAX_COMPONENTS), m_ComponentArray(), m_TypeHashMap(), m_OrderedIndex(), m_GameObject(gameObject), m_IsGameObject(false)
	{
		
	}

	ComponentManager::ComponentManager(ComponentManager&& other)
		: m_ComponentIdManager(std::move(other.m_ComponentIdManager)), m_TypeHashMap(std::move(other.m_TypeHashMap)), m_OrderedIndex(std::move(other.m_OrderedIndex)), m_GameObject(other.m_GameObject), m_IsGameObject(other.m_IsGameObject)
	{
		for (id_t index : m_OrderedIndex)
		{
			m_ComponentArray[index] = std::move(other.m_ComponentArray[index]);
		}
	}

	ComponentManager& ComponentManager::operator=(ComponentManager&& other)
	{
		m_ComponentIdManager = std::move(other.m_ComponentIdManager);
		m_TypeHashMap = std::move(other.m_TypeHashMap);
		m_GameObject = other.m_GameObject;
		m_IsGameObject = other.m_IsGameObject;
		m_OrderedIndex = std::move(other.m_OrderedIndex);
		for (id_t index : m_OrderedIndex)
		{
			m_ComponentArray[index] = std::move(other.m_ComponentArray[index]);
		}
		return *this;
	}

	ComponentManager::~ComponentManager()
	{
		BLT_PROFILE_FUNCTION();
		for (id_t index : m_OrderedIndex)
		{
			m_ComponentArray[index].component->End();
		}
	}

	Component& ComponentManager::GetComponentById(id_t id) const
	{
		BLT_PROFILE_FUNCTION();
		return *m_ComponentArray[id].component;
	}

	Component& ComponentManager::GetComponent(size_t componentTypeHash) const
	{
		BLT_PROFILE_FUNCTION();
		BLT_ASSERT(HasComponentPrivate(componentTypeHash), "Does not have given component type");
		return *m_ComponentArray[m_TypeHashMap.at(componentTypeHash)].component;
	}

	bool ComponentManager::GetComponent(size_t componentTypeHash, Component** outComponent) const
	{
		BLT_PROFILE_FUNCTION();
		auto it = m_TypeHashMap.find(componentTypeHash);
		if (it == m_TypeHashMap.end())
		{
			// Didn't find component
			*outComponent = nullptr;
			return false;
		}
		*outComponent = m_ComponentArray[it->second].component.get();
		return true;
	}

	bool ComponentManager::HasComponentById(id_t id) const
	{
		return (!m_ComponentIdManager.IsIdAvailable(id)) && (m_ComponentArray[id].component->IsEnabled());
	}

	bool ComponentManager::HasComponent(size_t componentTypeHash) const
	{
		BLT_PROFILE_FUNCTION();
		if (m_TypeHashMap.empty())
		{
			return false;
		}
		auto it = m_TypeHashMap.find(componentTypeHash);
		return (it != m_TypeHashMap.end()) && (m_ComponentArray[it->second].component->IsEnabled());
	}

	std::vector<Component*> ComponentManager::GetComponents() const
	{
		BLT_PROFILE_FUNCTION();
		std::vector<Component*> result;
		for (id_t index : m_OrderedIndex)
		{
			result.push_back(m_ComponentArray[index].component.get());
		}
		return result;
	}

	std::vector<ComponentManager::ComponentInfoPtr> ComponentManager::GetComponentsOrdered() const
	{
		BLT_PROFILE_FUNCTION();
		std::vector<ComponentInfoPtr> result;
		for (id_t index : m_OrderedIndex)
		{
			result.push_back({ m_ComponentArray[index].component.get(), m_ComponentArray[index].type_hash });
		}
		return result;
	}

	Component* ComponentManager::AddComponent(size_t componentTypeHash, std::unique_ptr<Component>&& component)
	{
		BLT_PROFILE_FUNCTION();
		if (HasComponentPrivate(componentTypeHash))
		{
			RemoveComponentById(m_TypeHashMap.at(componentTypeHash));
		}
		id_t id = FindNextId();
		component->m_Id = id;
		m_ComponentArray[id] = { std::move(component), componentTypeHash };
		m_TypeHashMap[componentTypeHash] = id;
		m_OrderedIndex.push_back(id);
		Component* c = m_ComponentArray[id].component.get();
		c->SetGameObject(m_GameObject);
		if (m_IsGameObject)
		{
			c->Start();
		}
		return c;
	}

	void ComponentManager::RemoveComponentById(id_t id)
	{
		BLT_PROFILE_FUNCTION();
		ComponentInfo& c = m_ComponentArray[id];
		c.component->End();
		c.component.reset(nullptr);
		m_TypeHashMap.erase(c.type_hash);
		auto it = std::find(m_OrderedIndex.begin(), m_OrderedIndex.end(), id);
		if (it != m_OrderedIndex.end())
		{
			m_OrderedIndex.erase(it);
		}
		m_ComponentIdManager.ReleaseId(id);
	}

	void ComponentManager::RemoveComponent(size_t componentTypeHash)
	{
		BLT_ASSERT(HasComponentPrivate(componentTypeHash), "Does not have given component type");
		id_t id = m_TypeHashMap.at(componentTypeHash);
		RemoveComponentById(id);
	}

	void ComponentManager::Clear()
	{
		BLT_PROFILE_FUNCTION();
		for (id_t index : m_OrderedIndex)
		{
			m_ComponentArray[index].component->End();
			m_ComponentArray[index].component.reset(nullptr);
		}
		m_TypeHashMap.clear();
		m_OrderedIndex.clear();
		m_ComponentIdManager.Reset();
	}

	void ComponentManager::Transfer(XMLserializer& backend, bool isWriting)
	{
		BLT_TRANSFER(backend, m_GameObject);
	}

	id_t ComponentManager::FindNextId() const
	{
		return m_ComponentIdManager.GetNextId();
	}

	void ComponentManager::SetGameObject(ObjectPrefab* object)
	{
		BLT_PROFILE_FUNCTION();
		m_GameObject = object;
		for (id_t index : m_OrderedIndex)
		{
			m_ComponentArray[index].component->SetGameObject(object);
		}
	}

	bool ComponentManager::HasComponentPrivate(size_t componentTypeHash) const
	{
		BLT_PROFILE_FUNCTION();
		if (m_TypeHashMap.empty())
		{
			return false;
		}
		auto it = m_TypeHashMap.find(componentTypeHash);
		return (it != m_TypeHashMap.end());
	}

}