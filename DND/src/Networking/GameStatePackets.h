#pragma once
#include "NetworkPackets.h"

#include "../Map/Tilemap.h"

namespace DND
{

	struct CharacterStats
	{
	public:
		int MaxHealth;
		int CurrentHealth;

		int Strength;
		int Dexterity;
		int Wisdom;

	public:
		inline static CharacterStats GetDelta(const CharacterStats& newStats, const CharacterStats& oldStats)
		{
			CharacterStats result;
			result.MaxHealth = newStats.MaxHealth - oldStats.MaxHealth;
			result.CurrentHealth = newStats.CurrentHealth - oldStats.CurrentHealth;
			result.Strength = newStats.Strength - oldStats.Strength;
			result.Dexterity = newStats.Dexterity - oldStats.Dexterity;
			result.Wisdom = newStats.Wisdom - oldStats.Wisdom;
			return result;
		}
	};

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
		CharacterStats Stats;
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