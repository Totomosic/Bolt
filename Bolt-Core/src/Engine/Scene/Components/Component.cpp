#include "Component.h"
#include "..\ObjectPrefab.h"
#include "..\GameObject.h"
#include "..\Layer.h"

namespace Bolt
{

	Component::Component() : Component(GameObject::InvalidID)
	{
	
	}

	Component::Component(id_t id)
		: m_Id(id), m_GameObject(nullptr)
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

	Layer* Component::layer() const
	{
		return gameObject()->GetLayer();
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

	void Component::SetGameObject(ObjectPrefab* object)
	{
		m_GameObject = object;
	}

}