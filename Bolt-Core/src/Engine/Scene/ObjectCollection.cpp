#include "ObjectCollection.h"

namespace Bolt
{

	void ObjectCollection::GameObjectInfo::Transfer(XMLserializer& backend, bool isWriting)
	{
		if (Enabled)
		{
			BLT_TRANSFER(backend, Object);
		}
	}

	ObjectCollection::ObjectCollection()
		: m_GameObjects{}, m_ActiveGameObjects()
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

	GameObject& ObjectCollection::GetGameObjectByTag(const blt::string& tag, int index) const
	{
		return *m_Tags.at(tag).at(index);
	}

	const std::vector<GameObject*>& ObjectCollection::GetGameObjectsByTag(const blt::string& tag) const
	{
		return m_Tags.at(tag);
	}

	bool ObjectCollection::TagExists(const blt::string& tag) const
	{
		return m_Tags.find(tag) != m_Tags.end();
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
		RemoveGameObject(object->Id());
	}

	void ObjectCollection::RemoveGameObject(id_t id)
	{
		GameObject* obj = &m_GameObjects[id].Object;
		RemoveAllTags(obj);
		m_GameObjects[id].Enabled = false;
		obj->SetID(GameObject::InvalidID);
		obj->SetLayer(nullptr);
		auto it = std::find(m_ActiveGameObjects.begin(), m_ActiveGameObjects.end(), obj);
		if (it != m_ActiveGameObjects.end())
		{
			m_ActiveGameObjects.erase(it);
		}		
		else
		{
			BLT_CORE_INFO("NOT IN ACTIVE OBJECTS");
		}
	}

	void ObjectCollection::RemoveAllWithTag(const blt::string& tag)
	{
		if (TagExists(tag))
		{
			std::vector<GameObject*> objects = GetGameObjectsByTag(tag);
			for (GameObject* object : objects)
			{
				RemoveGameObject(object);
			}
		}
	}

	void ObjectCollection::TagGameObject(const blt::string& tag, GameObject* object) const
	{
		m_Tags[tag].push_back(object);
		object->AddTagPrivate(tag);
	}

	void ObjectCollection::RemoveAllTags(GameObject* object) const
	{
		for (const blt::string& tag : object->Tags())
		{
			std::vector<GameObject*>& vector = m_Tags.at(tag);
			auto it = std::find(vector.begin(), vector.end(), object);
			vector.erase(it);
		}
		object->m_Tags.clear();
	}

	void ObjectCollection::RemoveTags(GameObject* object, size_t count) const
	{
		for (size_t i = 0; i < count; i++)
		{
			const blt::string& tag = object->Tags().at(i);
			std::vector<GameObject*>& vector = m_Tags.at(tag);
			auto it = std::find(vector.begin(), vector.end(), object);
			vector.erase(it);
		}
		object->m_Tags.clear();
	}

	void ObjectCollection::RemoveTag(GameObject* object, const blt::string& tag) const
	{
		BLT_ASSERT(std::find(object->Tags().begin(), object->Tags().end(), tag) != object->Tags().end(), "GameObject did not contain tag " + tag);
		auto it = std::find(object->Tags().begin(), object->Tags().end(), tag);
		object->m_Tags.erase(it);
		std::vector<GameObject*>& vector = m_Tags.at(tag);
		auto it2 = std::find(vector.begin(), vector.end(), object);
		vector.erase(it2);
	}

	void ObjectCollection::Transfer(XMLserializer& backend, bool isWriting)
	{
		BLT_TRANSFER_ARRAY(backend, m_GameObjects, MAX_GAMEOBJECTS);
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