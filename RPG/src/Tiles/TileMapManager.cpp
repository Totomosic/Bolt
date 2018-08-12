#include "TileMapManager.h"
#include "..\Entities\PlayerWizard.h"

namespace RPG
{

	TileMapManager::TileMapManager()
		: m_TileMaps(), m_LevelConnections(), m_CurrentTileMap(nullptr)
	{
		m_TileMaps.reserve(20);
	}

	TileMapLevel& TileMapManager::CurrentTileMap() const
	{
		return *m_CurrentTileMap;
	}

	const TileMapLevel& TileMapManager::GetTileMap(int index) const
	{
		return m_TileMaps[index];
	}

	TileMapLevel& TileMapManager::GetTileMap(int index)
	{
		return m_TileMaps[index];
	}

	TileMapLevel* TileMapManager::AddTileMap(TileMapLevel&& tilemap)
	{
		id_t index = m_TileMaps.size();
		m_TileMaps.push_back(std::move(tilemap));
		TileMapLevel* ptr = &m_TileMaps.at(index);
		return ptr;
	}

	void TileMapManager::SetCurrentTileMap(TileMapLevel* tilemap)
	{
		if (m_CurrentTileMap != nullptr)
		{
			m_CurrentTileMap->DestroyNPCs();
			m_CurrentTileMap->Destroy();
		}
		m_CurrentTileMap = tilemap;
		m_CurrentTileMap->CreateNPCs();
	}

	void TileMapManager::SetCurrentTileMap(id_t index)
	{
		SetCurrentTileMap(&m_TileMaps[index]);
	}

	void TileMapManager::AddConnection(TileMapLevel* level, TileMapConnection connection)
	{
		m_LevelConnections[level].push_back(connection);
	}

	void TileMapManager::AddTwoWayConnection(TileMapLevel* level, TileMapConnection connection)
	{
		AddConnection(level, connection);
		CharacterOrientation orientation = (connection.RequiredOrientation == CharacterOrientation::Right) ? CharacterOrientation::Left : (connection.RequiredOrientation == CharacterOrientation::Left) ? CharacterOrientation::Right : (connection.RequiredOrientation == CharacterOrientation::Up) ? CharacterOrientation::Down : CharacterOrientation::Down;
		TileMapConnection newConnection;
		newConnection.DestinationLevel = level;
		newConnection.DestinationPosition = connection.TriggerPosition;
		newConnection.TriggerPosition = connection.DestinationPosition;
		newConnection.RequiredOrientation = orientation;
		AddConnection(connection.DestinationLevel, newConnection);
	}

	void TileMapManager::Update(PlayerWizard* player)
	{
		if (m_LevelConnections.find(&CurrentTileMap()) != m_LevelConnections.end())
		{
			std::vector<TileMapConnection>& thisLevelConnections = m_LevelConnections.at(&CurrentTileMap());
			for (TileMapConnection& connection : thisLevelConnections)
			{
				if (player->CurrentTile() == &CurrentTileMap().GetTile(connection.TriggerPosition.x, connection.TriggerPosition.y) && player->Orientation() == connection.RequiredOrientation)
				{
					SetCurrentTileMap(connection.DestinationLevel);
					player->SetCurrentTile(&CurrentTileMap().GetTile(connection.DestinationPosition.x, connection.DestinationPosition.y));
				}
			}
		}
		m_CurrentTileMap->Update(player);
	}

}