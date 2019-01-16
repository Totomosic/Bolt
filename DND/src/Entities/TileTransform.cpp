#include "bltpch.h"
#include "TileTransform.h"

namespace DND
{

	TileTransform::TileTransform(Tilemap* tilemap, Tile currentTile, Vector3f positionOffset) : Component(),
		m_CurrentTile(currentTile), m_PositionOffset(positionOffset), m_Tilemap(tilemap)
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

	Tilemap& TileTransform::GetTilemap() const
	{
		return *m_Tilemap;
	}

	Vector3f TileTransform::PositionOfTile() const
	{
		return GetTilemap().WorldPositionOfTile(CurrentTile().x, CurrentTile().y);
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

	std::unique_ptr<Component> TileTransform::Clone() const
	{
		return std::make_unique<TileTransform>(m_Tilemap, m_CurrentTile, m_PositionOffset);
	}

	void TileTransform::UpdatePosition() const
	{
		Vector3f tilepos = PositionOfTile() + PositionOffset();
		gameObject()->transform().SetLocalPosition(tilepos);
	}

}