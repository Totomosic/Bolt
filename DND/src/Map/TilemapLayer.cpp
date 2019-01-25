#include "bltpch.h"
#include "TilemapLayer.h"

namespace DND
{

	TilemapLayer::TilemapLayer() : TilemapLayer(1, 1)
	{
	
	}

	TilemapLayer::TilemapLayer(int width, int height)
		: m_Width(width), m_Height(height), m_TileIds(std::make_unique<id_t[]>(width * height))
	{
		
	}

	int TilemapLayer::Width() const
	{
		return m_Width;
	}
	
	int TilemapLayer::Height() const
	{
		return m_Height;
	}

	id_t* TilemapLayer::Tiles() const
	{
		return m_TileIds.get();
	}

	id_t TilemapLayer::GetTile(int x, int y) const
	{
		return m_TileIds[x + y * Width()];
	}

	id_t& TilemapLayer::GetTile(int x, int y)
	{
		return m_TileIds[x + y * Width()];
	}

	void TilemapLayer::SetRegion(int x, int y, int width, int height, id_t tileId)
	{
		for (int i = x; i < x + width; i++)
		{
			for (int j = y; j < y + height; j++)
			{
				GetTile(i, j) = tileId;
			}
		}
	}

}