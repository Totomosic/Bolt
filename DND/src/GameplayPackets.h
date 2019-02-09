#pragma once
#include "ScenePackets.h"

namespace DND
{

	struct MoveToTilePacket
	{
	public:
		static constexpr PacketType Type = PacketType::MoveToTile;

	public:
		id_t NetworkId;
		Tile MoveTile;
	};

	template<>
	inline void Serialize(OutputMemoryStream& stream, const MoveToTilePacket& value)
	{
		Serialize(stream, value.NetworkId);
		Serialize(stream, value.MoveTile);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, MoveToTilePacket& value)
	{
		Deserialize(stream, value.NetworkId);
		Deserialize(stream, value.MoveTile);
	}

}