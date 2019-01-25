#pragma once
#include "Tilemap.h"
#include "Tileset.h"

namespace DND
{

	class TilemapManager
	{
	public:
		struct TilemapInfo
		{
		public:
			Tilemap Map;
			bool IsLoaded;
		};

		using MapLoadedCallback = std::function<void(id_t)>;

	private:
		IdManager<id_t> m_MapIdManager;
		std::unordered_map<id_t, TilemapInfo> m_Maps;
		id_t m_CurrentMap;
		Tileset m_Tileset;

		ObjectFactory m_Factory;
		int m_TileWidth;
		int m_TileHeight;

	public:
		TilemapManager(Layer& layer, int tileWidth, int tileHeight);

		const Tileset& TileImages() const;
		Tileset& TileImages();
		id_t CurrentMapId() const;
		const Tilemap& CurrentMap() const;
		Tilemap& CurrentMap();
		int TileWidth() const;
		int TileHeight() const;

		void SetCurrentMap(id_t mapId);
		id_t AddMap(Tilemap tilemap);

		void LoadTilemap(id_t mapId);
		void UnloadTilemap(id_t mapId);
		//void LoadTilemapAsync(id_t mapId, MapLoadedCallback callback = MapLoadedCallback());

	};

}