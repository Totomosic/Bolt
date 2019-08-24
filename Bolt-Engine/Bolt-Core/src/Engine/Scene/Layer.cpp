#include "bltpch.h"
#include "Layer.h"
#include "ObjectCollection.h"
#include "Core/Time/Time.h"

namespace Bolt
{

	Layer::Layer()
		: m_Id(GameObject::InvalidID), m_GameObjects(), m_Enabled(false), m_IsActive(false), m_UIManager(this), m_ActiveCamera(nullptr)
	{

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

	const UIManager& Layer::UI() const
	{
		return m_UIManager;
	}

	UIManager& Layer::UI()
	{
		return m_UIManager;
	}

	bool Layer::IsEnabled() const
	{
		return m_Enabled;
	}

	bool Layer::IsActive() const
	{
		return m_IsActive;
	}

	void Layer::Enable()
	{
		SetEnabled(true);
	}

	void Layer::Disable()
	{
		SetEnabled(false);
	}

	void Layer::SetEnabled(bool isEnabled)
	{
		m_Enabled = isEnabled;
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
		if (m_GameObjects.IsValidId(object->Id()))
		{
			m_GameObjects.RemoveGameObject(object);
		}
	}

	void Layer::RemoveGameObject(id_t id)
	{
		if (m_GameObjects.IsValidId(id))
		{
			RemoveGameObject(&m_GameObjects.GetGameObjectById(id));
		}
	}

	void Layer::Clear()
	{
		m_UIManager.Clear();
		std::vector<GameObject*> objects = m_GameObjects.GetAllGameObjects();
		for (GameObject* object : objects)
		{
			RemoveGameObject(object);
		}
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
			tObj.TimeToDelete -= Time::Get().RenderingTimeline().DeltaTime();
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

	void Layer::Create(id_t id, int maxGameObjects)
	{
		m_Id = id;
		Enable();
		m_GameObjects.Initialize(maxGameObjects);
		m_UIManager.Initialize();
	}

	void Layer::SetIsActive(bool isActive)
	{
		m_IsActive = isActive;
		if (isActive)
		{
			m_UIManager.OnActivate();
		}
		else
		{
			m_UIManager.OnDeactivate();
		}
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