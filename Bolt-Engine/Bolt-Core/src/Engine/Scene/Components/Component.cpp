#include "bltpch.h"

#include "Component.h"
#include "../ObjectPrefab.h"
#include "../GameObject.h"
#include "../Camera.h"
#include "../Layer.h"

namespace Bolt
{

	Component::Component() : Component(GameObject::InvalidID)
	{
	
	}

	Component::Component(id_t id)
		: m_Id(id), m_GameObject(nullptr), m_IsEnabled(true)
	{
		
	}

	Component::~Component()
	{

	}

	ObjectPrefab* Component::objectPrefab() const
	{
		return m_GameObject;
	}

	GameObject* Component::gameObject() const
	{
		return (GameObject*)m_GameObject;
	}

	Camera* Component::camera() const
	{
		if (layer() == nullptr)
		{
			// This object must be the camera
			return (Camera*)gameObject();
		}
		return layer()->ActiveCamera();
	}

	Layer* Component::layer() const
	{
		return gameObject()->GetLayer();
	}

	bool Component::IsEnabled() const
	{
		return m_IsEnabled;
	}

	id_t Component::Id() const
	{
		return m_Id;
	}

	void Component::Enable()
	{
		SetEnabled(true);
	}

	void Component::Disable()
	{
		SetEnabled(false);
	}

	void Component::SetEnabled(bool isEnabled)
	{
		m_IsEnabled = isEnabled;
	}

	void Component::Start()
	{
	
	}

	void Component::Update()
	{

	}

	void Component::LateUpdate()
	{
	
	}

	void Component::End()
	{
	
	}

	void Component::Transfer(XMLserializer& backend, bool isWriting)
	{
		BLT_TRANSFER(backend, m_Id);
		BLT_TRANSFER(backend, m_GameObject);
	}

	void Component::SetGameObject(ObjectPrefab* object)
	{
		m_GameObject = object;
	}

}