#include "ObjectCollection.h"

namespace Bolt
{

	ObjectCollection::ObjectCollection()
		: m_GameObjects(), m_ActiveGameObjects()
	{
		
	}

	const GameObject& ObjectCollection::GetGameObjectById(id_t id) const
	{
		return m_GameObjects[id].Object;
	}

	GameObject& ObjectCollection::GetGameObjectById(id_t id)
	{
		return m_GameObjects[id].Object;
	}

	const std::vector<GameObject*>& ObjectCollection::GetAllGameObjects() const
	{
		return m_ActiveGameObjects;
	}

	id_t ObjectCollection::AddGameObject(GameObject&& object)
	{
		id_t id = FindNextId();
		m_GameObjects[id] = { std::move(object), true };
		m_GameObjects[id].Object.SetID(id);
		m_ActiveGameObjects.push_back(&m_GameObjects[id].Object);
		return id;
	}

	void ObjectCollection::RemoveGameObject(GameObject* object)
	{
		RemoveGameObject(object->ID());
	}

	void ObjectCollection::RemoveGameObject(id_t id)
	{
		GameObject* obj = &m_GameObjects[id].Object;
		m_GameObjects[id].Enabled = false;
		m_GameObjects[id].Object.SetID(GameObject::InvalidID);
		m_GameObjects[id].Object.SetLayer(nullptr);
		auto it = std::find(m_ActiveGameObjects.begin(), m_ActiveGameObjects.end(), obj);
		if (it != m_ActiveGameObjects.end())
		{
			m_ActiveGameObjects.erase(it);
		}
	}

	id_t ObjectCollection::FindNextId() const
	{
		for (id_t i = 0; i < ObjectCollection::MAX_GAMEOBJECTS; i++)
		{
			if (!m_GameObjects[i].Enabled)
			{
				return i;
			}
		}
		BLT_ASSERT(false, "Unable to find valid Id for GameObject");
		return GameObject::InvalidID;
	}

}