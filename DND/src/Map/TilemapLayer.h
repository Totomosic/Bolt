#pragma once
#include "Bolt.h"

namespace DND
{

	class TilemapLayer
	{
	private:
		int m_Width;
		int m_Height;
		int m_TileWidth;
		int m_TileHeight;
		int m_PixelsPerTileX;
		int m_PixelsPerTileY;
		Texture2D m_Texture;		

	public:
		TilemapLayer();
		TilemapLayer(int width, int height, int tileWidth, int tileHeight, float tilemapResolution);

		const Texture2D& GetTexture() const;
		Texture2D& GetTexture();
		int Width() const;
		int Height() const;
		Vector2i TileSize() const;
		Vector2i PixelsPerTile() const;

		void SetTileImage(int x, int y, const Image& image, ResizeFilter filter);
		void SetTileImages(int x, int y, int w, int h, const Image& image, ResizeFilter filter);

	};

}