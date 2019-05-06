#include "MeteorController.h"

namespace Meteors
{

	MeteorController::MeteorController(float radius, float gravity, const Vector2f& velocity, MeteorController::MeteorDestroyFunc onDestroy) : Component(),
		m_Gravity(gravity), m_Radius(radius), m_Velocity(velocity), m_OnDestroy(onDestroy)
	{
		
	}

	float MeteorController::GetRadius() const
	{
		return m_Radius;
	}

	void MeteorController::Update()
	{
		Transform& t = gameObject()->transform();
		m_Velocity.y -= m_Gravity * Time::Get().RenderingTimeline().DeltaTime();
		t.Translate({ m_Velocity * Time::Get().RenderingTimeline().DeltaTime(), 0 });
		t.Rotate(-m_Velocity.x / (PI * m_Radius) * Time::Get().RenderingTimeline().DeltaTime() * 3, Vector3f::Forward());

		if (t.Position().y - m_Radius < FloorHeight || (t.Position().x + m_Radius <= 0 && m_Velocity.x < 0) || (t.Position().x - m_Radius >= ViewWidth && m_Velocity.x > 0))
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