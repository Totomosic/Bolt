#pragma once
#include "bltpch.h"
#include "../../../Map/TilemapManager.h"

namespace DND
{

	class CurrentMap : public Component
	{
	private:
		id_t m_CurrentMapId;
		TilemapManager* m_MapManager;

	public:
		CurrentMap(id_t currentMapId, TilemapManager* mapManager);

		id_t MapId() const;
		Tilemap& Map() const;

		void Start() override;
		void SetMapId(id_t mapId);
		void CurrentMapChanged(id_t currentMapId);

		std::unique_ptr<Component> Clone() const override;

	};

}