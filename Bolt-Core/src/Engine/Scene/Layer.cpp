#include "Layer.h"
#include "SceneGraph\__SceneGraph__.h"

namespace Bolt
{

	Layer::Layer()
		: m_Id(GameObject::InvalidID), m_GameObjects(), m_SceneGraph(std::make_unique<Bolt::SceneArray>()), m_Enabled(false), m_ActiveCamera(nullptr)
	{
		m_SceneGraph->SetObjectCollection(&m_GameObjects);
	}

	const ObjectCollection& Layer::GameObjects() const
	{
		return m_GameObjects;
	}

	const SceneGraph& Layer::Graph() const
	{
		return *m_SceneGraph;
	}

	Camera* Layer::ActiveCamera() const
	{
		return m_ActiveCamera;
	}

	id_t Layer::Id() const
	{
		return m_Id;
	}

	bool Layer::IsEnabled() const
	{
		return m_Enabled;
	}

	void Layer::Enable()
	{
		m_Enabled = true;
	}

	void Layer::Disable()
	{
		m_Enabled = false;
	}

	void Layer::SetActiveCamera(Camera* camera)
	{
		m_ActiveCamera = camera;
	}

	GameObject* Layer::AddGameObject(GameObject&& object)
	{
		id_t id = m_GameObjects.AddGameObject(std::move(object));
		GameObject* obj = &m_GameObjects.GetGameObjectById(id);
		m_SceneGraph->AddGameObject(obj);
		obj->SetLayer(this);
		return obj;
	}

	void Layer::RemoveGameObject(GameObject* object)
	{
		if (object->ID() < ObjectCollection::MAX_GAMEOBJECTS)
		{
			m_GameObjects.RemoveGameObject(object);
		}
	}

	void Layer::RemoveGameObject(id_t id)
	{
		if (id < ObjectCollection::MAX_GAMEOBJECTS)
		{
			RemoveGameObject(&m_GameObjects.GetGameObjectById(id));
		}
	}

	void Layer::Clear()
	{
		std::vector<GameObject*> objects = m_SceneGraph->GetAllGameObjects();
		for (GameObject* object : objects)
		{
			RemoveGameObject(object);
		}
	}

	void Layer::Update()
	{
		std::vector<GameObject*> validObjects;
		for (id_t i = 0; i < ObjectCollection::MAX_GAMEOBJECTS; i++)
		{
			GameObject& object = m_GameObjects.GetGameObjectById(i);
			if (object.ID() != GameObject::InvalidID)
			{
				object.Update();
				validObjects.push_back(&object);
			}
		}
		for (GameObject* object : validObjects)
		{
			object->LateUpdate();
		}
	}

	void Layer::UpdateTemporaryObjects()
	{
		for (int i = m_TemporaryObjects.size() - 1; i >= 0; i--)
		{
			TempGameObject& tObj = m_TemporaryObjects[i];
			tObj.TimeToDelete -= Time::DeltaTime();
			if (tObj.TimeToDelete <= 0.0f)
			{
				RemoveGameObject(tObj.Object);
				m_TemporaryObjects.erase(m_TemporaryObjects.begin() + i);
			}
		}
	}

	GameObject* Layer::AddTemporaryGameObject(GameObject&& object)
	{
		GameObject* obj = AddGameObject(std::move(object));
		MarkGameObjectForDelete(obj, 0.0f);
		return obj;
	}

	void Layer::MarkGameObjectForDelete(GameObject* object, float timeToDelete)
	{
		m_TemporaryObjects.push_back({ object, timeToDelete });
	}

	void Layer::Transfer(XMLserializer& backend, bool isWriting)
	{
		BLT_TRANSFER(backend, m_Id);
		BLT_TRANSFER(backend, m_GameObjects);
		BLT_TRANSFER(backend, m_ActiveCamera);
	}

	void Destroy(GameObject* object, float timeToDelete)
	{
		BLT_ASSERT(object->GetLayer() != nullptr, "Attempted to destroy GameObject that is not part of a layer");
		object->GetLayer()->MarkGameObjectForDelete(object, timeToDelete);
	}

}