#pragma once
#include "Bolt.h"
#include "Constants.h"

namespace Meteors
{

	class PlayerMovement : public Component
	{
	private:
		float m_Speed;
		float m_JumpHeight;
		float m_Gravity;
		int m_JumpCount;
		int m_MaxJumpCount;

		Vector2f m_Velocity;

	public:
		PlayerMovement(float speed, float jumpHeight, float gravity, int jumpCount);

		void Update() override;

		BLT_DEFAULT_COMPONENT_CLONE(PlayerMovement);

	};

}