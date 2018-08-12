#pragma once
#include "TileMapLevel.h"
#include "TileMapConnection.h"

namespace RPG
{
	
	class PlayerWizard;

	class TileMapManager
	{
	private:
		std::vector<TileMapLevel> m_TileMaps;
		std::unordered_map<TileMapLevel*, std::vector<TileMapConnection>> m_LevelConnections;
		TileMapLevel* m_CurrentTileMap;

	public:
		TileMapManager();

		TileMapLevel& CurrentTileMap() const;
		const TileMapLevel& GetTileMap(int index) const;
		TileMapLevel& GetTileMap(int index);

		TileMapLevel* AddTileMap(TileMapLevel&& tilemap);
		void SetCurrentTileMap(TileMapLevel* tilemap);
		void SetCurrentTileMap(id_t index);

		void AddConnection(TileMapLevel* level, TileMapConnection connection);
		void AddTwoWayConnection(TileMapLevel* level, TileMapConnection connection);

		void Update(PlayerWizard* player);

	};

}