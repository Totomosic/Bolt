#include "bltpch.h"
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
		Image im;
		im.Width = image.Width * w;
		im.Height = image.Height * h;
		im.Components = 4;
		im.Pixels = new byte[im.Width * im.Height * im.Components];
		for (int j = 0; j < h; j++)
		{
			for (int h = 0; h < image.Height; h++)
			{
				for (int i = 0; i < w; i++)
				{
					memcpy(im.Pixels + i * image.Width * 4 + j * im.Width* image.Height * 4 + h * im.Width * 4, image.Pixels + h * image.Width * 4, image.Width * 4);
				}
			}
		}
		m_Texture.SetRegion(x * PixelsPerTile().x, y * PixelsPerTile().y, w * PixelsPerTile().x, h * PixelsPerTile().y, im, filter);
	}

}