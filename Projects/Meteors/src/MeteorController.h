#pragma once
#include "Bolt.h"
#include "Constants.h"

namespace Meteors
{

	class MeteorController : public Component
	{
	public:
		using MeteorDestroyFunc = std::function<void(GameObject*, const Vector3f&)>;

	private:
		float m_Gravity;
		float m_Radius;
		Vector2f m_Velocity;
		MeteorDestroyFunc m_OnDestroy;

	public:
		MeteorController(float radius, float gravity, const Vector2f& velocity, MeteorDestroyFunc onDestroy);

		float GetRadius() const;
		void Update() override;
		void Destroy();

		BLT_DEFAULT_COMPONENT_CLONE(MeteorController);

	};

}