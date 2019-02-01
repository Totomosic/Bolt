#include "bltpch.h"
#include "DndInstance.h"

namespace DND
{

	DndInstance::DndInstance(Layer& layer, const ResourcePtr<Texture2D>& tileset) : Component(),
		m_Tilemap(layer, 50, 50)
	{
		id_t grassTileId = m_Tilemap.TileImages().AddTile(tileset->GetImage(0, 0, 32, 32));
		id_t pathTileId = m_Tilemap.TileImages().AddTile(tileset->GetImage(0, 32, 32, 32));

		Tilemap map0(50, 50);
		TilemapLayer& map0Layer = map0.AddLayer(1.0f);
		map0Layer.SetRegion(0, 0, map0Layer.Width(), map0Layer.Height(), grassTileId);
		map0Layer.SetRegion(22, 0, 6, map0Layer.Height(), pathTileId);
		m_Tilemap.AddMap(std::move(map0));
	}

	void DndInstance::Start()
	{
		m_Tilemap.LoadTilemap(0);
		m_Tilemap.SetCurrentMap(0);		
	}

	void DndInstance::End()
	{

	}

	std::unique_ptr<Component> DndInstance::Clone() const
	{
		BLT_ASSERT(false, "Cannot clone Dnd Instance");
		return std::unique_ptr<DndInstance>();
	}

}