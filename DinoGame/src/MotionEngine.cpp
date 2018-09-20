#include "MotionEngine.h"

namespace DinoGame
{

	MotionEngine::MotionEngine(const float* gameSpeed, const bool* isPaused) : Component(),
		m_GameSpeed(gameSpeed), m_IsPaused(isPaused), Velocity(0.0f)
	{

	}

	void MotionEngine::Update()
	{
		if (!*m_IsPaused)
		{
			Transform& t = gameObject()->transform();
			t.Translate(*m_GameSpeed * -Time::DeltaTime(), 0, 0);
			t.Translate(Velocity.x * Time::DeltaTime(), Velocity.y * Time::DeltaTime(), 0);
			if (t.Position().x < -50)
			{
				Destroy(gameObject(), 0.0f);
			}
		}
	}

	std::unique_ptr<Component> MotionEngine::Clone() const
	{
		std::unique_ptr<MotionEngine> comp = std::make_unique<MotionEngine>(m_GameSpeed, m_IsPaused);
		comp->Velocity = Velocity;
		return std::move(comp);
	}

}