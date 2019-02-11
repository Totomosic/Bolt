#include "bltpch.h"
#include "CurrentMap.h"

namespace DND
{

	CurrentMap::CurrentMap(id_t mapId, TilemapManager* mapManager) : Component(),
		m_CurrentMapId(mapId), m_MapManager(mapManager)
	{
	
	}

	id_t CurrentMap::MapId() const
	{
		return m_CurrentMapId;
	}

	Tilemap& CurrentMap::Map() const
	{
		return m_MapManager->GetMap(MapId());
	}

	void CurrentMap::Start()
	{
		CurrentMapChanged(m_MapManager->CurrentMapId());
	}

	void CurrentMap::SetMapId(id_t mapId)
	{
		m_CurrentMapId = mapId;
		CurrentMapChanged(m_MapManager->CurrentMapId());
	}

	void CurrentMap::CurrentMapChanged(id_t currentMapId)
	{
		MeshRenderer* mr;
		if (gameObject()->Components().GetComponent(&mr))
		{
			mr->SetEnabled(currentMapId == MapId());
		}
	}

	std::unique_ptr<Component> CurrentMap::Clone() const
	{
		return std::make_unique<CurrentMap>(MapId(), m_MapManager);
	}

}