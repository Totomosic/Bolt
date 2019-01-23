#pragma once
#include "NetworkPackets.h"

#include "../Map/Tilemap.h"
#include "Entities/Characters/CharacterStats.h"

namespace DND
{

	struct ConnectionInfo
	{
	public:
		AddressPair Address;
	};

	struct CharacterInfo
	{
	public:
		id_t NetworkId;
		id_t PrefabId;
		Tile CurrentTile;
	};

	struct NetworkPlayerInfo
	{
	public:
		ConnectionInfo Connection;
		id_t PlayerId;
		CharacterInfo Character;
	};

	struct HelloPacket
	{
	public:
		constexpr static PacketType Type = PacketType::Hello;
	};

	struct WelcomePacket
	{
	public:
		constexpr static PacketType Type = PacketType::Welcome;

	public:
		id_t PlayerId;
		id_t NetworkId;
		id_t NextPlayerId;
		id_t NextNetworkId;
		std::vector<NetworkPlayerInfo> Players;
		std::vector<CharacterInfo> OtherCharacters;
	};

	struct IntroductionPacket
	{
	public:
		constexpr static PacketType Type = PacketType::Introduction;

	public:
		NetworkPlayerInfo Player;
	};

	struct NotAcceptingClientsPacket
	{
	public:
		constexpr static PacketType Type = PacketType::NotAcceptingClients;
	};

	struct PlayerDisconnectPacket
	{
	public:
		constexpr static PacketType Type = PacketType::PlayerDisconnect;

	public:
		id_t PlayerId;
	};

}