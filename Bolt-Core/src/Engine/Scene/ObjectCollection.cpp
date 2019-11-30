#include "bltpch.h"

#include "ObjectCollection.h"
#include "Core/Profiling/Profiling.h"

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
		: m_IdManager(0, 0), m_GameObjectCapacity(0), m_GameObjects(), m_ActiveGameObjects()
	{
		
	}

	void ObjectCollection::Initialize(int maxGameObjects)
	{
		BLT_ASSERT(maxGameObjects > RESERVED_GAMEOBJECTS, "Must contain more than {} GameObjects", RESERVED_GAMEOBJECTS);
		m_IdManager = IdManager<id_t>(RESERVED_GAMEOBJECTS, maxGameObjects - 1);
		m_GameObjectCapacity = maxGameObjects;
		m_GameObjects = std::make_unique<GameObjectInfo[]>(maxGameObjects);
		CreateReservedGameObjects();
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
		return m_Tags[tag];
	}

	bool ObjectCollection::TagExists(const blt::string& tag) const
	{
		return m_Tags.find(tag) != m_Tags.end();
	}

	bool ObjectCollection::IsValidId(id_t id) const
	{
		return id < m_GameObjectCapacity;
	}

	id_t ObjectCollection::AddGameObject(GameObject&& object)
	{
		id_t id = FindNextId();
		m_GameObjects[id] = { std::move(object), true };
		m_GameObjects[id].Object.SetId(id);
		m_ActiveGameObjects.push_back(&m_GameObjects[id].Object);
		return id;
	}

	void ObjectCollection::RemoveGameObject(GameObject* object)
	{
		RemoveGameObject(object->Id());
	}

	void ObjectCollection::RemoveGameObject(id_t id)
	{
		BLT_PROFILE_FUNCTION();
		if (id < ObjectCollection::RESERVED_GAMEOBJECTS)
		{
			BLT_CORE_ERROR("Attempted to BLT_DELETE reserved GameObject with Id: {}", id);
			return;
		}
		GameObject* obj = &m_GameObjects[id].Object;
		obj->OnDestroy();
		RemoveAllTags(obj);		
		m_GameObjects[id].Enabled = false;
		ReleaseId(id);
		auto it = std::find(m_ActiveGameObjects.begin(), m_ActiveGameObjects.end(), obj);
		if (it != m_ActiveGameObjects.end())
		{
			m_ActiveGameObjects.erase(it);
		}		
		else
		{
			BLT_CORE_WARN("DELTED GAMEOBJECT NOT IN ACTIVE OBJECTS");
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

	void ObjectCollection::RemoveTags(GameObject* object, uint32_t count) const
	{
		for (uint32_t i = 0; i < count; i++)
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

	SGQueryResult ObjectCollection::Query(const SGQuery& query) const
	{
		SGQueryResult result;
		result.GameObjects = query.Evaluate(*this, &result.MostRelevant, &result.LeastRelevant);
		return result;
	}

	void ObjectCollection::Reset()
	{
		// 0 = UI object for each layer which is never deleted
		m_IdManager.Reset(ObjectCollection::RESERVED_GAMEOBJECTS);
	}

	void ObjectCollection::Transfer(XMLserializer& backend, bool isWriting)
	{
		
	}

	id_t ObjectCollection::FindNextId() const
	{
		id_t i = m_IdManager.GetNextId();
		return i;
	}

	void ObjectCollection::ReleaseId(id_t id) const
	{
		m_IdManager.ReleaseId(id);
	}

	void ObjectCollection::CreateReservedGameObjects()
	{
		for (id_t i = 0; i < ObjectCollection::RESERVED_GAMEOBJECTS; i++)
		{
			m_GameObjects[i].Enabled = true;
			m_GameObjects[i].Object.SetId(i);
			//m_ActiveGameObjects.push_back(&m_GameObjects[i].Object);
		}
	}

}