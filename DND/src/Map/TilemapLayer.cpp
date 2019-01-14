#include "TilemapLayer.h"

namespace DND
{

	TilemapLayer::TilemapLayer()
		: m_Texture(0, 0)
	{
	
	}

	TilemapLayer::TilemapLayer(int width, int height, int tileWidth, int tileHeight, float tilemapResolution)
		: m_Width(width), m_Height(height), m_TileWidth(tileWidth), m_TileHeight(tileHeight), m_PixelsPerTileX(tileWidth * tilemapResolution), m_PixelsPerTileY(tileHeight * tilemapResolution), m_Texture(m_PixelsPerTileX * m_Width, m_PixelsPerTileY * m_Height)
	{
		
	}

	const Texture2D& TilemapLayer::GetTexture() const
	{
		return m_Texture;
	}

	Texture2D& TilemapLayer::GetTexture()
	{
		return m_Texture;
	}

	int TilemapLayer::Width() const
	{
		return m_Width;
	}
	
	int TilemapLayer::Height() const
	{
		return m_Height;
	}

	Vector2i TilemapLayer::TileSize() const
	{
		return { m_TileWidth, m_TileHeight };
	}

	Vector2i TilemapLayer::PixelsPerTile() const
	{
		return { m_PixelsPerTileX, m_PixelsPerTileY };
	}

	void TilemapLayer::SetTileImage(int x, int y, const Image& image, ResizeFilter filter)
	{
		m_Texture.SetRegion(x * PixelsPerTile().x, y * PixelsPerTile().y, PixelsPerTile().x, PixelsPerTile().y, image, filter);
	}

	void TilemapLayer::SetTileImages(int x, int y, int w, int h, const Image& image, ResizeFilter filter)
	{
		for (int i = x; i < x + w; i++)
		{
			for (int j = y; j < y + h; j++)
			{
				SetTileImage(i, j, image, filter);
			}
		}
	}

}