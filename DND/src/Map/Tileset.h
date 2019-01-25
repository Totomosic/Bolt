#pragma once
#include "bltpch.h"

namespace DND
{

	struct Tileset
	{
	private:
		IdManager<id_t> m_TileIdManager;
		std::unordered_map<id_t, Image> m_Tiles;

	public:
		Tileset();

		const std::unordered_map<id_t, Image>& TileImages() const;
		const Image& GetTile(id_t tileId) const;
		Image& GetTile(id_t tileId);

		id_t AddTile(Image&& image);

	};

}