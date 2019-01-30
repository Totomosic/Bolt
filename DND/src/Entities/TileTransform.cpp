#include "bltpch.h"
#include "TileTransform.h"
#include "../GameManager.h"

namespace DND
{

	TileTransform::TileTransform(id_t mapId, Tile currentTile, Vector3f positionOffset) : Component(),
		m_CurrentTile(currentTile), m_PositionOffset(positionOffset), m_MapId(mapId)
	{
	
	}

	const Tile& TileTransform::CurrentTile() const
	{
		return m_CurrentTile;
	}

	const Vector3f& TileTransform::PositionOffset() const
	{
		return m_PositionOffset;
	}

	Vector3f TileTransform::PositionOfTile() const
	{
		return GameManager::Get().MapManager().GetMap(m_MapId).WorldPositionOfTile(CurrentTile().x, CurrentTile().y);
	}

	id_t TileTransform::CurrentMapId() const
	{
		return m_MapId;
	}

	void TileTransform::Start()
	{
		UpdatePosition();
	}

	void TileTransform::SetCurrentTile(Tile tile, bool updatePosition)
	{
		m_CurrentTile = std::move(tile);
		if (updatePosition)
		{
			UpdatePosition();
		}
	}

	void TileTransform::SetPositionOffset(Vector3f positionOffset)
	{
		m_PositionOffset = positionOffset;
	}

	void TileTransform::SetCurrentMapId(id_t mapId)
	{
		m_MapId = mapId;
	}

	std::unique_ptr<Component> TileTransform::Clone() const
	{
		return std::make_unique<TileTransform>(m_MapId, m_CurrentTile, m_PositionOffset);
	}

	void TileTransform::UpdatePosition() const
	{
		Vector3f tilepos = PositionOfTile() + PositionOffset();
		Vector3f localPos = gameObject()->transform().LocalPosition();
		gameObject()->transform().SetLocalPosition(tilepos.x, tilepos.y, localPos.z);
	}

}