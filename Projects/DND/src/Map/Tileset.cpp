#include "bltpch.h"
#include "Tileset.h"

namespace DND
{

	Tileset::Tileset()
		: m_TileIdManager(0, GameObject::InvalidID - 1), m_Tiles()
	{
	
	}

	const std::unordered_map<id_t, Image>& Tileset::TileImages() const
	{
		return m_Tiles;
	}

	const Image& Tileset::GetTile(id_t tileId) const
	{
		return m_Tiles.at(tileId);
	}

	Image& Tileset::GetTile(id_t tileId)
	{
		return m_Tiles.at(tileId);
	}

	id_t Tileset::AddTile(Image&& image)
	{
		id_t id = m_TileIdManager.GetNextId();
		m_Tiles[id] = std::move(image);
		return id;
	}

}