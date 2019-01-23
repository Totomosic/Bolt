#pragma once
#include "NetworkPackets.h"
#include "../Map/Tilemap.h"

namespace DND
{

	struct EntityMovedPacket
	{
	public:
		constexpr static PacketType Type = PacketType::EntityMoved;

	public:
		id_t NetworkId;
		Tile MoveToTile;
	};

}