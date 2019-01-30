#pragma once
#include "GameStatePacketSerialization.h"
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

	struct CastSpellPacket
	{
	public:
		constexpr static PacketType Type = PacketType::CastSpell;

	public:
		id_t NetworkId;
		id_t SpellId;
		OutputMemoryStream SpellData;
	};

	struct StatsUpdatePacket
	{
	public:
		constexpr static PacketType Type = PacketType::StatsUpdate;

	public:
		id_t NetworkId;
		CharacterStats NewStats;
	};

	struct CreateNPCPacket
	{
	public:
		constexpr static PacketType Type = PacketType::CreateNPC;

	public:
		id_t OwnerNetworkId;
		CharacterInfo Character;
	};

	struct DestroyNPCPacket
	{
	public:
		constexpr static PacketType Type = PacketType::DestroyNPC;

	public:
		id_t NetworkId;
	};

}