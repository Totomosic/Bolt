#include "bltpch.h"
#include "TileTransform.h"

namespace DND
{

	TileTransform::TileTransform(const Tile& bottomLeftTile) : TileTransform(bottomLeftTile, 1, 1)
	{
	
	}

	TileTransform::TileTransform(const Tile& bottomLeftTile, int width, int height) : Component(),
		m_CurrentMap(nullptr), m_BottomLeftTile(bottomLeftTile), m_Width(width), m_Height(height)
	{
		
	}

	const Tile& TileTransform::BottomLeftTile() const
	{
		return m_BottomLeftTile;
	}

	Tile TileTransform::TopRightTile() const
	{
		return BottomLeftTile() + Tile(Width() - 1, Height() - 1);
	}

	int TileTransform::Width() const
	{
		return m_Width;
	}

	int TileTransform::Height() const
	{
		return m_Height;
	}

	Vector3f TileTransform::WorldPositionOfBottomLeft() const
	{
		return m_CurrentMap->Map().WorldPositionOfTile(BottomLeftTile());
	}

	Vector3f TileTransform::WorldPositionOfTopRight() const
	{
		return m_CurrentMap->Map().WorldPositionOfTile(TopRightTile());
	}

	Vector3f TileTransform::WorldPositionOfCenter() const
	{
		return (WorldPositionOfBottomLeft() + WorldPositionOfTopRight()) / 2.0f;
	}

	Vector3f TileTransform::WorldPositionOfTile(const Tile& tile) const
	{
		return m_CurrentMap->Map().WorldPositionOfTile(tile);
	}

	bool TileTransform::IsTileInMap(const Tile& tile) const
	{
		return (tile.x >= 0 && tile.y >= 0 && tile.x < m_CurrentMap->Map().Width() && tile.y < m_CurrentMap->Map().Height());
	}

	void TileTransform::Start()
	{
		m_CurrentMap = &gameObject()->Components().GetComponent<CurrentMap>();
		UpdateWorldPosition();
	}

	void TileTransform::SetBottomLeftTile(const Tile& bottomLeftTile, bool updateWorldPosition)
	{
		m_BottomLeftTile = bottomLeftTile;
		if (updateWorldPosition)
		{
			UpdateWorldPosition();
		}
	}

	void TileTransform::SetWidth(int width, bool updateWorldPosition)
	{
		SetSize({ width, Height() }, updateWorldPosition);
	}

	void TileTransform::SetHeight(int height, bool updateWorldPosition)
	{
		SetSize({ Width(), height }, updateWorldPosition);
	}

	void TileTransform::SetSize(const Vector2i& size, bool updateWorldPosition)
	{
		m_Width = size.x;
		m_Height = size.y;
		if (updateWorldPosition)
		{
			UpdateWorldPosition();
		}
	}

	void TileTransform::UpdateWorldPosition()
	{
		gameObject()->transform().SetLocalXY(WorldPositionOfCenter().xy());
	}

	std::unique_ptr<Component> TileTransform::Clone() const
	{
		return std::make_unique<TileTransform>(m_BottomLeftTile, m_Width, m_Height);
	}

}