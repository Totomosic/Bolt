#include "MeteorController.h"

namespace Meteors
{

	MeteorController::MeteorController(float radius, float gravity, const Vector2f& velocity, MeteorController::MeteorDestroyFunc onDestroy) : Component(),
		m_Gravity(gravity), m_Radius(radius), m_Velocity(velocity), m_OnDestroy(onDestroy)
	{
		
	}

	void MeteorController::Update()
	{
		Transform& t = gameObject()->transform();
		m_Velocity.y -= m_Gravity * Time::Get().RenderingTimeline().DeltaTime();
		t.Translate({ m_Velocity * Time::Get().RenderingTimeline().DeltaTime(), 0 });

		if (t.Position().y - m_Radius < FloorHeight)
		{
			Destroy();
		}
	}

	void MeteorController::Destroy()
	{
		m_OnDestroy(gameObject(), gameObject()->transform().Position());
		Bolt::Destroy(gameObject());
	}

}