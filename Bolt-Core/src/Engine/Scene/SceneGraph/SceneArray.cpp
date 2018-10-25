#include "SceneArray.h"

namespace Bolt
{

	SceneArray::SceneArray() : SceneGraph(),
		m_GameObjects(), m_MaxGameObjectID(0)
	{

	}

	id_t SceneArray::AddGameObject(GameObject* gameObject)
	{
		id_t id = gameObject->Id();
		m_GameObjects[id] = gameObject;
		if ((int)id > m_MaxGameObjectID)
		{
			m_MaxGameObjectID = id;
		}
		return id;
	}

	const GameObject& SceneArray::GetGameObjectById(id_t id) const
	{
		return *m_GameObjects[id];
	}

	GameObject& SceneArray::GetGameObjectById(id_t id)
	{
		return *m_GameObjects[id];
	}

	std::vector<GameObject*> SceneArray::GetAllGameObjects() const
	{
		std::vector<GameObject*> result;
		for (int i = 0; i <= m_MaxGameObjectID; i++)
		{
			if (m_GameObjects[i] != nullptr && m_GameObjects[i]->Id() != GameObject::InvalidID)
			{
				result.push_back(m_GameObjects[i]);
			}
		}
		return result;
	}

	SGQueryResult SceneArray::Query(const SGQuery& query) const
	{
		SGQueryResult result;
		result.GameObjects = query.Evaluate(*this, &result.MostRelevant, &result.LeastRelevant);
		return result;
	}

}