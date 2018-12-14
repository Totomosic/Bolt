#pragma once
#include "PhysObjCollection.h"
#include "CollisionEngine\CollisionEngine.h"

namespace Bolt
{

	class BLT_API PhysicsEngine
	{
	private:
		PhysObjCollection m_Objects;
		CollisionEngine m_CollisionEngine;

	public:
		PhysicsEngine();
		PhysicsEngine(const PhysicsEngine& other);
		PhysicsEngine& operator=(const PhysicsEngine& other);
		PhysicsEngine(PhysicsEngine&& other);
		PhysicsEngine& operator=(PhysicsEngine&& other);
		~PhysicsEngine() = default;

		const PhysObjCollection& Objects() const;
		void AddGameObject(GameObject* object);
		void RemoveGameObject(GameObject* object);
		void Clear();

		void Update(double elapsedSeconds);

	};

}