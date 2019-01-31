#include "bltpch.h"
#include "TileMapTracker.h"
#include "../GameManager.h"

namespace DND
{

	TileMapTracker::TileMapTracker(id_t mapId) : Component(),
		m_MapId(mapId)
	{
	
	}

	id_t TileMapTracker::MapId() const
	{
		return m_MapId;
	}

	void TileMapTracker::SetMapId(id_t mapId)
	{
		m_MapId = mapId;
		CurrentMapChanged(GameManager::Get().MapManager().CurrentMapId());
	}

	void TileMapTracker::CurrentMapChanged(id_t newMapId)
	{
		gameObject()->Components().GetComponent<MeshRenderer>().SetEnabled(newMapId == m_MapId);
	}

	void TileMapTracker::Start()
	{
		CurrentMapChanged(GameManager::Get().MapManager().CurrentMapId());
	}

	std::unique_ptr<Component> TileMapTracker::Clone() const
	{
		return std::make_unique<TileMapTracker>(m_MapId);
	}

}