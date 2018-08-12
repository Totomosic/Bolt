#include "RigidBody.h"

namespace MeteorGame
{

	RigidBody::RigidBody() : Component(),
		Velocity(), Acceleration(), zAngle(0.0f)
	{

	}

	void RigidBody::Update()
	{
		Velocity += Acceleration * Time::DeltaTime();
		gameObject()->transform().Translate(Velocity * Time::DeltaTime());
		gameObject()->transform().Rotate(zAngle * Time::DeltaTime(), Vector3f::Forward());
	}

	std::unique_ptr<Component> RigidBody::Clone() const
	{
		std::unique_ptr<RigidBody> rb = std::make_unique<RigidBody>();
		rb->Velocity = Velocity;
		rb->Acceleration = Acceleration;
		rb->zAngle = zAngle;
		return std::move(rb);
	}

}