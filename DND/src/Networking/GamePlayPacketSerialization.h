#pragma once
#include "GamePlayPackets.h"
#include "GameStatePacketSerialization.h"

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

	template<>
	inline void Serialize(OutputMemoryStream& stream, const CastSpellPacket& value)
	{
		Serialize(stream, value.NetworkId);
		Serialize(stream, value.SpellId);
		Serialize(stream, value.SpellData);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, CastSpellPacket& value)
	{
		Deserialize(stream, value.NetworkId);
		Deserialize(stream, value.SpellId);
		Deserialize(stream, value.SpellData);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const StatsUpdatePacket& value)
	{
		Serialize(stream, value.NetworkId);
		Serialize(stream, value.NewStats);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, StatsUpdatePacket& value)
	{
		Deserialize(stream, value.NetworkId);
		Deserialize(stream, value.NewStats);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const CreateNPCPacket& value)
	{
		Serialize(stream, value.OwnerNetworkId);
		Serialize(stream, value.Character);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, CreateNPCPacket& value)
	{
		Deserialize(stream, value.OwnerNetworkId);
		Deserialize(stream, value.Character);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const DestroyNPCPacket& value)
	{
		Serialize(stream, value.NetworkId);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, DestroyNPCPacket& value)
	{
		Deserialize(stream, value.NetworkId);
	}

}