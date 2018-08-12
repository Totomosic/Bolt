#include "ObjectCollection.h"

namespace Bolt
{

	ObjectCollection::ObjectCollection()
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

	id_t ObjectCollection::AddGameObject(GameObject&& object)
	{
		id_t id = FindNextId();
		m_GameObjects[id] = { std::move(object), true };
		m_GameObjects[id].Object.SetID(id);
		return id;
	}

	void ObjectCollection::RemoveGameObject(GameObject* object)
	{
		RemoveGameObject(object->ID());
	}

	void ObjectCollection::RemoveGameObject(id_t id)
	{
		m_GameObjects[id].Enabled = false;
		m_GameObjects[id].Object.SetID(GameObject::InvalidID);
		m_GameObjects[id].Object.SetLayer(nullptr);
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