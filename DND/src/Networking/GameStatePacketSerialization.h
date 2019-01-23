#pragma once
#include "GameStatePackets.h"

namespace DND
{

	template<>
	inline void Serialize(OutputMemoryStream& stream, const ConnectionInfo& value)
	{
		Serialize(stream, value.Address);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, ConnectionInfo& value)
	{
		Deserialize(stream, value.Address);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const CharacterInfo& value)
	{
		Serialize(stream, value.NetworkId);
		Serialize(stream, value.PrefabId);
		Serialize(stream, value.CurrentTile);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, CharacterInfo& value)
	{
		Deserialize(stream, value.NetworkId);
		Deserialize(stream, value.PrefabId);
		Deserialize(stream, value.CurrentTile);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const NetworkPlayerInfo& value)
	{
		Serialize(stream, value.Connection);
		Serialize(stream, value.PlayerId);
		Serialize(stream, value.Character);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, NetworkPlayerInfo& value)
	{
		Deserialize(stream, value.Connection);
		Deserialize(stream, value.PlayerId);
		Deserialize(stream, value.Character);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const HelloPacket& value)
	{
		
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, HelloPacket& value)
	{

	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const WelcomePacket& value)
	{
		Serialize(stream, value.PlayerId);
		Serialize(stream, value.NetworkId);
		Serialize(stream, value.NextPlayerId);
		Serialize(stream, value.NextNetworkId);
		Serialize(stream, value.Players);
		Serialize(stream, value.OtherCharacters);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, WelcomePacket& value)
	{
		Deserialize(stream, value.PlayerId);
		Deserialize(stream, value.NetworkId);
		Deserialize(stream, value.NextPlayerId);
		Deserialize(stream, value.NextNetworkId);
		Deserialize(stream, value.Players);
		Deserialize(stream, value.OtherCharacters);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const IntroductionPacket& value)
	{
		Serialize(stream, value.Player);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, IntroductionPacket& value)
	{
		Deserialize(stream, value.Player);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const NotAcceptingClientsPacket& value)
	{

	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, NotAcceptingClientsPacket& value)
	{

	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const PlayerDisconnectPacket& value)
	{
		Serialize(stream, value.PlayerId);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, PlayerDisconnectPacket& value)
	{
		Deserialize(stream, value.PlayerId);
	}

}