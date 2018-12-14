#include "MapTile.h"
#include "GameManager.h"

namespace Blockstream
{

	MapTile::MapTile(int level) : Component(),
		m_Level(level), m_TargetLevel(level), m_Water(false), m_End(false)
	{
	
	}

	void MapTile::Start()
	{
		SetLevel(m_Level);
	}

	void MapTile::Update()
	{
		Vector3f currentPosition = gameObject()->transform().Position();
		Vector3f targetPosition = Vector3f(currentPosition.x, m_TargetLevel * HEIGHT_PER_LEVEL, currentPosition.z);
		if (std::abs(currentPosition.y - targetPosition.y) <= 0.001f)
		{
			if (m_Level != m_TargetLevel)
			{
				m_Level = m_TargetLevel;
				gameObject()->transform().SetLocalPosition(targetPosition);
				GameManager::Server().OnBlockMoved.Post();
			}
			return;
		}
		else
		{
			float vecLength = (targetPosition - currentPosition).Length();
			float length = (vecLength < TILE_SPEED * Time::RenderingTimeline().DeltaTime()) ? vecLength : TILE_SPEED * Time::RenderingTimeline().DeltaTime();
			gameObject()->transform().Translate((targetPosition - currentPosition).Normalize() * length);
		}
	}

	int MapTile::GetLevel() const
	{
		return m_Level;
	}

	bool MapTile::IsFlooded() const
	{
		return m_Water;
	}

	void MapTile::SetLevel(int level)
	{
		m_TargetLevel = level;
	}

	void MapTile::IncrementLevel()
	{
		SetLevel(m_TargetLevel + 1);
	}

	void MapTile::DecrementLevel()
	{
		SetLevel(m_TargetLevel - 1);
	}

	void MapTile::SetWater(bool flooded)
	{
		m_Water = flooded;
		if (!m_End)
		{
			if (m_Water)
			{
				gameObject()->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].BaseColor = Color::Blue;
			}
			else
			{
				gameObject()->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].BaseColor = Color::White;
			}
		}
	}

	void MapTile::MakeEndTile()
	{
		gameObject()->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].BaseColor = Color::Red;
		m_End = true;
	}

	std::unique_ptr<Component> MapTile::Clone() const
	{
		return std::make_unique<MapTile>(m_Level);
	}

}