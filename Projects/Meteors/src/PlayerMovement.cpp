#include "PlayerMovement.h"

namespace Meteors
{

	PlayerMovement::PlayerMovement(float speed, float jumpHeight, float gravity, int jumpCount) : Component(),
		m_Speed(speed), m_JumpHeight(jumpHeight), m_Gravity(gravity), m_JumpCount(jumpCount), m_MaxJumpCount(jumpCount), m_Velocity()
	{
	
	}

	void PlayerMovement::Update()
	{
		Transform& t = gameObject()->transform();
		MeshRenderer& m = gameObject()->Components().GetComponent<MeshRenderer>();
		if (Input::Get().KeyDown(Keycode::A))
		{
			m_Velocity.x = -m_Speed;
			m.Mesh.Materials[0]->GetLinkContext().Link("Texture", ResourceManager::Get().GetResource<Texture2D>(LeftFacingCharacterTexture));
		}
		else if (Input::Get().KeyDown(Keycode::D))
		{
			m_Velocity.x = m_Speed;
			m.Mesh.Materials[0]->GetLinkContext().Link("Texture", ResourceManager::Get().GetResource<Texture2D>(RightFacingCharacterTexture));
		}
		else
		{
			m_Velocity.x = 0;
		}
		if (Input::Get().KeyPressed(Keycode::W) && m_JumpCount > 0 && m_Velocity.y < m_JumpHeight * (m_JumpCount / (float)m_MaxJumpCount))
		{
			m_Velocity.y = m_JumpHeight * (m_JumpCount / (float)m_MaxJumpCount);
			m_JumpCount--;
		}

		if (t.Position().y - CharacterHeight / 2 > FloorHeight)
		{
			m_Velocity.y -= m_Gravity * Time::Get().RenderingTimeline().DeltaTime();
		}

		t.Translate({ m_Velocity * Time::Get().RenderingTimeline().DeltaTime(), 0 });
		if (t.Position().x - CharacterWidth / 2 < 0)
		{
			t.SetLocalX(CharacterWidth / 2);
		}
		if (t.Position().x + CharacterWidth / 2 > ViewWidth)
		{
			t.SetLocalX(ViewWidth - CharacterWidth / 2);
		}
		if (t.Position().y - CharacterHeight / 2 < FloorHeight)
		{
			t.SetLocalY(FloorHeight + CharacterHeight / 2);
			m_Velocity.y = 0;
			m_JumpCount = m_MaxJumpCount;
		}
	}

}