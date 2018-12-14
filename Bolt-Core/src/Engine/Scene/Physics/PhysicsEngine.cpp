#include "Types.h"
#include "PhysicsEngine.h"

namespace Bolt
{

	PhysicsEngine::PhysicsEngine()
		: m_Objects(), m_CollisionEngine(&m_Objects)
	{
	
	}

	PhysicsEngine::PhysicsEngine(const PhysicsEngine& other)
		: m_Objects(other.m_Objects), m_CollisionEngine(&m_Objects)
	{
	
	}

	PhysicsEngine& PhysicsEngine::operator=(const PhysicsEngine& other)
	{
		m_Objects = other.m_Objects;
		m_CollisionEngine = CollisionEngine(&m_Objects);
		return *this;
	}

	PhysicsEngine::PhysicsEngine(PhysicsEngine&& other)
		: m_Objects(other.m_Objects), m_CollisionEngine(&m_Objects)
	{
		other.m_Objects.clear();
	}

	PhysicsEngine& PhysicsEngine::operator=(PhysicsEngine&& other)
	{
		m_Objects = std::move(other.m_Objects);
		m_CollisionEngine = CollisionEngine(&m_Objects);
		return *this;
	}

	const PhysObjCollection& PhysicsEngine::Objects() const
	{
		return m_Objects;
	}

	void PhysicsEngine::AddGameObject(GameObject* object)
	{
		m_Objects.push_back(object);
	}

	void PhysicsEngine::RemoveGameObject(GameObject* object)
	{
		auto it = std::find(m_Objects.begin(), m_Objects.end(), object);
		if (it != m_Objects.end())
		{
			m_Objects.erase(it);
			return;
		}
		BLT_CORE_ERROR("Unable to find GameObject with id {0} in PhysicsEngine when removing it", object->Id());
	}

	void PhysicsEngine::Clear()
	{
		m_Objects.clear();
	}

	void PhysicsEngine::Update(double elapsedSeconds)
	{
		
	}

}