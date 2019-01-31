#pragma once
#include "bltpch.h"

namespace DND
{

	class TileMapTracker : public Component
	{
	private:
		id_t m_MapId;
		
	public:
		TileMapTracker(id_t mapId);

		id_t MapId() const;
		void SetMapId(id_t mapId);
		void CurrentMapChanged(id_t newMapId);
		
		void Start() override;

		std::unique_ptr<Component> Clone() const override;

	};

}