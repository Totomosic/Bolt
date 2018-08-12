#include "Component.h"
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

	GameObject* Component::gameObject() const
	{
		return m_GameObject;
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

	void Component::SetGameObject(GameObject* object)
	{
		m_GameObject = object;
	}

}