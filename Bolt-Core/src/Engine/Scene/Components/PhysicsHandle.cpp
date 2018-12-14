#include "Types.h"
#include "PhysicsHandle.h"
#include "..\SceneManager.h"

namespace Bolt
{

	PhysicsHandle::PhysicsHandle() : Component(),
		PhysEngineId(GameObject::InvalidID)
	{
	
	}

	PhysicsHandle::~PhysicsHandle()
	{
		SceneManager::CurrentScene().Physics().RemoveGameObject(gameObject());
	}

	void PhysicsHandle::Start()
	{
		PhysEngineId = SceneManager::CurrentScene().Physics().AddGameObject(gameObject());
	}

	std::unique_ptr<Component> PhysicsHandle::Clone() const
	{
		return std::make_unique<PhysicsHandle>();
	}

}