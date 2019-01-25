#pragma once
#include "bltpch.h"

namespace DND
{

	using Tile = Vector2i;

	class TilemapLayer
	{
	private:
		int m_Width;
		int m_Height;
		std::unique_ptr<id_t[]> m_TileIds;

	public:
		TilemapLayer();
		TilemapLayer(int width, int height);

		int Width() const;
		int Height() const;
		id_t* Tiles() const;
		id_t GetTile(int x, int y) const;
		id_t& GetTile(int x, int y);

		void SetRegion(int x, int y, int width, int height, id_t tileId);

	};

}