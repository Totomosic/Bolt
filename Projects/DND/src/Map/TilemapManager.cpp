#include "bltpch.h"
#include "TilemapManager.h"

namespace DND
{

	TilemapManager::TilemapManager(int tileWidth, int tileHeight)
		: m_MapIdManager(0, GameObject::InvalidID - 1), m_Maps(), m_CurrentMap((id_t)-1), m_Tileset(), m_Factory(), m_TileWidth(tileWidth), m_TileHeight(tileHeight)
	{
	
	}

	const Tileset& TilemapManager::TileImages() const
	{
		return m_Tileset;
	}

	Tileset& TilemapManager::TileImages()
	{
		return m_Tileset;
	}

	id_t TilemapManager::CurrentMapId() const
	{
		return m_CurrentMap;
	}

	const Tilemap& TilemapManager::CurrentMap() const
	{
		return m_Maps.at(m_CurrentMap).Map;
	}

	Tilemap& TilemapManager::CurrentMap()
	{
		return m_Maps.at(m_CurrentMap).Map;
	}

	const Tilemap& TilemapManager::GetMap(id_t mapId) const
	{
		return m_Maps.at(mapId).Map;
	}

	Tilemap& TilemapManager::GetMap(id_t mapId)
	{
		return m_Maps.at(mapId).Map;
	}

	int TilemapManager::TileWidth() const
	{
		return m_TileWidth;
	}

	int TilemapManager::TileHeight() const
	{
		return m_TileHeight;
	}

	bool TilemapManager::IsLoaded(id_t mapId) const
	{
		return m_Maps.at(mapId).IsLoaded;
	}

	void TilemapManager::SetMapLayer(Layer& layer)
	{
		m_Factory.SetCurrentLayer(layer);
	}

	void TilemapManager::SetCurrentMap(id_t mapId)
	{
		if (m_CurrentMap != GameObject::InvalidID)
		{
			CurrentMap().Hide();
		}
		m_CurrentMap = mapId;
		CurrentMap().Show();
	}

	id_t TilemapManager::AddMap(Tilemap map)
	{
		id_t id = m_MapIdManager.GetNextId();
		map.SetTileSize({ m_TileWidth, m_TileHeight });
		m_Maps[id] = { std::move(map), false };
		return id;
	}

	void TilemapManager::LoadTilemap(id_t mapId)
	{
		TilemapInfo& map = m_Maps.at(mapId);
		map.Map.Load(m_Factory, TileImages().TileImages());
		map.IsLoaded = true;
	}

	void TilemapManager::LoadTilemapAsync(id_t mapId, std::function<void()> callback)
	{
		TilemapInfo& map = m_Maps.at(mapId);
		map.Map.LoadAsync(m_Factory, TileImages().TileImages(), [&map, callback = std::move(callback)]()
		{
			map.IsLoaded = true;
			callback();
		});
		
	}

	void TilemapManager::UnloadTilemap(id_t mapId)
	{
		TilemapInfo& map = m_Maps.at(mapId);
		map.Map.Unload();
		map.IsLoaded = false;
	}

	void TilemapManager::UnloadAllTilemaps()
	{
		for (auto& pair : m_Maps)
		{
			if (pair.second.IsLoaded)
			{
				pair.second.Map.Unload();
			}
		}
	}

}