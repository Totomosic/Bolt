#pragma once
#include "../../../Network/Serialization.h"
#include "../../../Network/Deserialization.h"
#include "../../../Map/TilemapLayer.h"

namespace DND
{

	struct EntityNetworkData
	{
	public:
		id_t NetworkId;
		id_t PrefabId;
		id_t MapId;
		Tile BottomLeftTile;
		std::vector<EntityNetworkData> OwnedObjects;

	};

	template<>
	inline void Serialize(OutputMemoryStream& stream, const EntityNetworkData& value)
	{
		Serialize(stream, value.NetworkId);
		Serialize(stream, value.PrefabId);
		Serialize(stream, value.MapId);
		Serialize(stream, value.BottomLeftTile);
		Serialize(stream, value.OwnedObjects);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, EntityNetworkData& value)
	{
		Deserialize(stream, value.NetworkId);
		Deserialize(stream, value.PrefabId);
		Deserialize(stream, value.MapId);
		Deserialize(stream, value.BottomLeftTile);
		Deserialize(stream, value.OwnedObjects);
	}

}