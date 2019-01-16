#include "bltpch.h"
#include "TileMotion.h"

namespace DND
{

	TileMotion::TileMotion(float speed) : Component(),
		m_TargetTile(), m_Speed(speed), m_Seeking(false), m_Transform(nullptr), m_Animator(nullptr)
	{
	
	}

	const Tile& TileMotion::TargetTile() const
	{
		return m_TargetTile;
	}

	float TileMotion::CurrentSpeed() const
	{
		return m_Speed;
	}
	
	bool TileMotion::IsMoving() const
	{
		return m_Seeking;
	}

	bool TileMotion::HasAnimator() const
	{
		return m_Animator != nullptr;
	}

	void TileMotion::Start()
	{
		TileTransform& t = gameObject()->Components().GetComponent<TileTransform>();
		m_TargetTile = t.CurrentTile();
		m_Transform = &t;
		FindAnimator();
	}

	void TileMotion::Update()
	{
		if (IsMoving())
		{
			Transform& t = gameObject()->transform();
			Vector3f targetPosition = m_Transform->GetTilemap().WorldPositionOfTile(m_TargetTile.x, m_TargetTile.y) + m_Transform->PositionOffset();
			Vector3f toTarget = targetPosition - t.Position();
			Vector3f dir = toTarget.Normalize();
			float length = std::min<float>(Time::RenderingTimeline().DeltaTime() * m_Speed, toTarget.Length());
			t.Translate(dir * length);
			if ((targetPosition - t.Position()).LengthSqrd() < 0.01f)
			{
				t.SetLocalPosition(targetPosition);
				m_Seeking = false;
			}
		}
	}

	void TileMotion::SetTargetTile(const Tile& targetTile)
	{
		Tile currentTile = m_Transform->CurrentTile();
		if (targetTile.x >= 0 && targetTile.x < m_Transform->GetTilemap().Width() && targetTile.y >= 0 && targetTile.y < m_Transform->GetTilemap().Height())
		{
			m_TargetTile = targetTile;
			m_Seeking = true;
			m_Transform->SetCurrentTile(m_TargetTile, false);
		}
		if (HasAnimator())
		{
			Tile diff = targetTile - currentTile;
			if (diff.x > 0)
			{
				m_Animator->SetDirection(Direction::Right);
			}
			else if (diff.x < 0)
			{
				m_Animator->SetDirection(Direction::Left);
			}
			else if (diff.y > 0)
			{
				m_Animator->SetDirection(Direction::Up);
			}
			else if (diff.y < 0)
			{
				m_Animator->SetDirection(Direction::Down);
			}
		}
	}

	void TileMotion::SetCurrentSpeed(float speed)
	{
		m_Speed = speed;
	}

	void TileMotion::FindAnimator()
	{
		if (gameObject()->Components().HasComponent<CharacterAnimator>())
		{
			m_Animator = &gameObject()->Components().GetComponent<CharacterAnimator>();
		}
	}

	std::unique_ptr<Component> TileMotion::Clone() const
	{
		return std::make_unique<TileMotion>(m_Speed);
	}

}