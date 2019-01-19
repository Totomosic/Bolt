#include "Types.h"

#include "Layer.h"
#include "ObjectCollection.h"

namespace Bolt
{

	Layer::Layer()
		: m_Id(GameObject::InvalidID), m_GameObjects(), m_Enabled(false), m_ActiveCamera(nullptr), m_UIroot()
	{

	}

	Layer::~Layer()
	{
		m_UIroot.ReleaseGameObjects();
		m_UIroot.Clear();
	}

	const ObjectCollection& Layer::GameObjects() const
	{
		return m_GameObjects;
	}

	ObjectCollection& Layer::GameObjects()
	{
		return m_GameObjects;
	}

	Camera* Layer::ActiveCamera() const
	{
		return m_ActiveCamera;
	}

	id_t Layer::Id() const
	{
		return m_Id;
	}

	const UIroot& Layer::UI() const
	{
		return m_UIroot;
	}

	UIroot& Layer::UI()
	{
		return m_UIroot;
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
		obj->SetLayer(this);
		return obj;
	}

	void Layer::RemoveGameObject(GameObject* object)
	{
		if (object->Id() < ObjectCollection::MAX_GAMEOBJECTS)
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
		std::vector<GameObject*> objects = m_GameObjects.GetAllGameObjects();
		for (GameObject* object : objects)
		{
			RemoveGameObject(object);
		}
		UI().ReleaseGameObjects();
		UI().Clear();
		m_GameObjects.Reset();
		m_TemporaryObjects.clear();
	}

	void Layer::Update()
	{
		std::vector<GameObject*> objects = m_GameObjects.GetAllGameObjects();
		for (GameObject* object : objects)
		{
			object->Update();
		}
		for (GameObject* object : objects)
		{
			object->LateUpdate();
		}
	}

	void Layer::UpdateTemporaryObjects()
	{
		for (int i = m_TemporaryObjects.size() - 1; i >= 0; i--)
		{
			TempGameObject& tObj = m_TemporaryObjects[i];
			tObj.TimeToDelete -= Time::RenderingTimeline().DeltaTime();
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

	void Layer::Create(id_t id)
	{
		m_Id = id;
		Enable();
		m_UIroot.GetFactory().SetCurrentLayer(*this);
		m_UIroot.m_Object = &m_GameObjects.GetGameObjectById(0);
	}

	void Destroy(GameObject* object, float timeToDelete)
	{
		if (object->GetLayer() == nullptr)
		{
			BLT_CORE_WARN("Deleting GameObject that either does not have a layer or may have already been deleted. Id = {0}", object->Id());
			return;
		}
		object->GetLayer()->MarkGameObjectForDelete(object, timeToDelete);
	}

}