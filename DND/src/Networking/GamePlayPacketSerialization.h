#pragma once
#include "GamePlayPackets.h"

namespace DND
{

	template<>
	inline void Serialize(OutputMemoryStream& stream, const EntityMovedPacket& value)
	{
		Serialize(stream, value.NetworkId);
		Serialize(stream, value.MoveToTile);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, EntityMovedPacket& value)
	{
		Deserialize(stream, value.NetworkId);
		Deserialize(stream, value.MoveToTile);
	}

}