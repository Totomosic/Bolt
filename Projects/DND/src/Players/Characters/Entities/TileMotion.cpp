#include "bltpch.h"
#include "TileMotion.h"

#include "CharacterAnimator.h"

namespace DND
{

	TileMotion::TileMotion() : TileMotion(2)
	{
	
	}

	TileMotion::TileMotion(float tilesPerSecond) : Component(),
		m_Transform(nullptr), m_Speed(tilesPerSecond), m_TargetTile(), m_IsSeeking(false)
	{
	
	}

	float TileMotion::TilesPerSecond() const
	{
		return m_Speed;
	}

	float TileMotion::RealSpeed() const
	{
		return TilesPerSecond() * gameObject()->Components().GetComponent<CurrentMap>().Map().TileWidth();
	}

	bool TileMotion::IsSeeking() const
	{
		return m_IsSeeking;
	}

	void TileMotion::Start()
	{
		m_Transform = &gameObject()->Components().GetComponent<TileTransform>();
	}

	void TileMotion::Update()
	{
		if (m_IsSeeking)
		{
			Vector3f currentPosition = gameObject()->transform().Position();
			Vector3f targetPosition = m_Transform->WorldPositionOfTile(m_TargetTile);
			Vector3f toTarget = targetPosition - currentPosition;
			float toTargetLength = toTarget.Length();
			Vector3f toTargetDir = toTarget.Normalize();
			float moveLength = RealSpeed() * Time::RenderingTimeline().DeltaTime();
			if (toTargetLength <= moveLength)
			{
				moveLength = toTargetLength;
				m_IsSeeking = false;
			}
			gameObject()->transform().Translate(toTargetDir * moveLength);
		}
	}

	void TileMotion::SetSpeed(float tilesPerSecond)
	{
		m_Speed = tilesPerSecond;
	}

	void TileMotion::MoveToBottomLeftTile(const Tile& targetTile)
	{
		Tile delta = targetTile - m_Transform->BottomLeftTile();
		if (m_Transform->IsTileInMap(targetTile))
		{
			m_IsSeeking = true;
			m_TargetTile = targetTile;
			m_Transform->SetBottomLeftTile(targetTile, false);
		}
		if (gameObject()->Components().HasComponent<CharacterAnimator>())
		{
			CharacterAnimator& animator = gameObject()->Components().GetComponent<CharacterAnimator>();
			if (delta.x < 0)
			{
				animator.SetCurrentDirection(Direction::Left);
			}
			else if (delta.x > 0)
			{
				animator.SetCurrentDirection(Direction::Right);
			}
			else if (delta.y < 0)
			{
				animator.SetCurrentDirection(Direction::Down);
			}
			else if (delta.y > 0)
			{
				animator.SetCurrentDirection(Direction::Up);
			}
		}
	}

	void TileMotion::JumpToBottomLeftTile(const Tile& targetTile)
	{
		if (m_Transform->IsTileInMap(targetTile))
		{
			m_Transform->SetBottomLeftTile(targetTile, true);
			m_IsSeeking = false;
			m_TargetTile = targetTile;
		}
	}

	std::unique_ptr<Component> TileMotion::Clone() const
	{
		return std::make_unique<TileMotion>(m_Speed);
	}

}