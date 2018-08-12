#pragma once
#include "Component.h"

namespace Bolt
{

	class BLT_API MotionEngine : public Component
	{
	public:
		Vector3f Velocity;
		Vector3f Acceleration;

	public:
		MotionEngine();
		MotionEngine(const Vector3f& velocity, const Vector3f& acceleration = Vector3f(0.0f));

		void Update() override;
		std::unique_ptr<Component> Clone() const override;

	};

}