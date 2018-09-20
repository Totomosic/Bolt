#include "JumpScript.h"

namespace DinoGame
{

	JumpScript::JumpScript(float gravity, const bool* isPaused) : Component(),
		m_Gravity(gravity), m_Velocity(0.0f), m_IsPaused(isPaused)
	{
	
	}

	void JumpScript::Start()
	{
		m_RestHeight = gameObject()->transform().Position().y;
	}

	void JumpScript::Update()
	{
		if (!*m_IsPaused)
		{
			Transform& t = gameObject()->transform();
			if (t.Position().y < m_RestHeight)
			{
				m_Velocity = 0;
				t.SetLocalPosition(t.Position().x, m_RestHeight, t.Position().z);
			}
			float moveAmount = m_Velocity * Time::DeltaTime();
			if (moveAmount < 0.0f)
			{
				moveAmount = max(moveAmount, m_RestHeight - t.Position().y);
			}
			t.Translate(0, moveAmount, 0);
			m_Velocity -= m_Gravity * Time::DeltaTime();
		}
	}

	void JumpScript::Jump(float power)
	{
		m_Velocity = power;
	}

	bool JumpScript::CanJump() const
	{
		return gameObject()->transform().Position().y == m_RestHeight;
	}

	std::unique_ptr<Component> JumpScript::Clone() const
	{
		return std::make_unique<JumpScript>(m_Gravity, m_IsPaused);
	}

}