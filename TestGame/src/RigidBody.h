#pragma once
#include "BoltEngine.h"

using namespace Bolt;

namespace MeteorGame
{

	class RigidBody : public Component
	{
	public:
		Vector3f Velocity;
		Vector3f Acceleration;
		float zAngle;

	public:
		RigidBody();

		void Update() override;

		std::unique_ptr<Component> Clone() const override;

	};

}