#pragma once
#include "Network/Packets.h"
#include "Network/Serialization.h"
#include "Network/Deserialization.h"

#include "Players/GameNetworkData.h"

namespace DND
{

	struct HelloPacket
	{
	public:
		static constexpr PacketType Type = PacketType::Hello;
	};

	template<>
	inline void Serialize(OutputMemoryStream& stream, const HelloPacket& value)
	{
	
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, HelloPacket& value)
	{
	
	}

	struct WelcomePacket
	{
	public:
		static constexpr PacketType Type = PacketType::Welcome;

	public:
		id_t PlayerId;
		id_t NetworkId;
		GameNetworkData GameState;
	};

	template<>
	inline void Serialize(OutputMemoryStream& stream, const WelcomePacket& value)
	{
		Serialize(stream, value.PlayerId);
		Serialize(stream, value.NetworkId);
		Serialize(stream, value.GameState);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, WelcomePacket& value)
	{
		Deserialize(stream, value.PlayerId);
		Deserialize(stream, value.NetworkId);
		Deserialize(stream, value.GameState);
	}

	struct IntroductionPacket
	{
	public:
		static constexpr PacketType Type = PacketType::Introduction;

	public:
		AddressPair Address;
		id_t PlayerId;
		EntityNetworkData PlayerObject;
	};

	template<>
	inline void Serialize(OutputMemoryStream& stream, const IntroductionPacket& value)
	{
		Serialize(stream, value.Address);
		Serialize(stream, value.PlayerId);
		Serialize(stream, value.PlayerObject);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, IntroductionPacket& value)
	{
		Deserialize(stream, value.Address);
		Deserialize(stream, value.PlayerId);
		Deserialize(stream, value.PlayerObject);
	}

	struct DisconnectPacket
	{
	public:
		static constexpr PacketType Type = PacketType::Disconnect;

	public:
		id_t PlayerId;
	};

	template<>
	inline void Serialize(OutputMemoryStream& stream, const DisconnectPacket& value)
	{
		Serialize(stream, value.PlayerId);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, DisconnectPacket& value)
	{
		Deserialize(stream, value.PlayerId);
	}

}