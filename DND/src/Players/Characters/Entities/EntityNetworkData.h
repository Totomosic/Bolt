#pragma once
#include "../../../Network/Serialization.h"
#include "../../../Network/Deserialization.h"
#include "../../../Map/TilemapLayer.h"
#include "EntityStats.h"

namespace DND
{

	struct EntityTransformData
	{
	public:
		Tile BottomLeftTile;
		int Width;
		int Height;
	};

	struct EntityNetworkData
	{
	public:
		id_t NetworkId;
		id_t PrefabId;
		id_t MapId;
		EntityTransformData TransformData;
		float TilesPerSecondSpeed;
		StatsNetworkData MaxStats;
		StatsNetworkData CurrentStats;
		std::vector<EntityNetworkData> OwnedObjects;

	};

	template<>
	inline void Serialize(OutputMemoryStream& stream, const EntityTransformData& value)
	{
		Serialize(stream, value.BottomLeftTile);
		Serialize(stream, value.Width);
		Serialize(stream, value.Height);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, EntityTransformData& value)
	{
		Deserialize(stream, value.BottomLeftTile);
		Deserialize(stream, value.Width);
		Deserialize(stream, value.Height);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const EntityNetworkData& value)
	{
		Serialize(stream, value.NetworkId);
		Serialize(stream, value.PrefabId);
		Serialize(stream, value.MapId);
		Serialize(stream, value.TransformData);
		Serialize(stream, value.TilesPerSecondSpeed);
		Serialize(stream, value.MaxStats);
		Serialize(stream, value.CurrentStats);
		Serialize(stream, value.OwnedObjects);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, EntityNetworkData& value)
	{
		Deserialize(stream, value.NetworkId);
		Deserialize(stream, value.PrefabId);
		Deserialize(stream, value.MapId);
		Deserialize(stream, value.TransformData);
		Deserialize(stream, value.TilesPerSecondSpeed);
		Deserialize(stream, value.MaxStats);
		Deserialize(stream, value.CurrentStats);
		Deserialize(stream, value.OwnedObjects);
	}

}