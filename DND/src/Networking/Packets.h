#pragma once
#include "bltpch.h"
#include "../Map/Tilemap.h"

#include "Serialization.h"
#include "Deserialization.h"

namespace DND
{

	enum class PacketType : byte
	{
		None,
		LocalSocketTerminate,
		Hello,
		Welcome,
		Introduce,
		Disconnect,

		PlayerMove,
		MAX_PACKET_TYPES
	};

	constexpr int USER_EVENT_ID = EventManager::USER_EVENT_ID + (int)PacketType::MAX_PACKET_TYPES + 1;

	inline int PacketTypeToEventId(PacketType type)
	{
		return EventManager::USER_EVENT_ID + (int)type;
	}	


	struct HelloPacket
	{
	public:
		int Placeholder;
	};

	struct CharacterInfo
	{
	public:
		id_t CharacterPrefabId;
		Tile CurrentTile;
		id_t NetworkId;
	};

	struct PlayerInfo
	{
	public:
		SocketAddress Address;
		id_t PlayerId;
		CharacterInfo Character;
	};

	struct WelcomePacket
	{
	public:
		id_t PlayerId;
		id_t NetworkId;
		std::vector<PlayerInfo> Players;
		std::vector<CharacterInfo> Characters;
	};

	struct IntroductionPacket
	{
	public:
		id_t PlayerId;
		CharacterInfo Character;
	};

	struct DisconnectPacket
	{

	};

	struct PlayerMovePacket
	{
	public:
		id_t NetworkId;
		Tile MoveToTile;
	};

	template<typename T>
	inline PacketType TypeOfPacket()
	{
		BLT_ASSERT(false, "Unable to determine type of packet " + typeid(T).name());
		return (PacketType)-1;
	}

	template<>
	inline PacketType TypeOfPacket<HelloPacket>()
	{
		return PacketType::Hello;
	}
	template<>
	inline PacketType TypeOfPacket<WelcomePacket>()
	{
		return PacketType::Welcome;
	}
	template<>
	inline PacketType TypeOfPacket<IntroductionPacket>()
	{
		return PacketType::Introduce;
	}
	template<>
	inline PacketType TypeOfPacket<DisconnectPacket>()
	{
		return PacketType::Disconnect;
	}
	template<>
	inline PacketType TypeOfPacket<PlayerMovePacket>()
	{
		return PacketType::PlayerMove;
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const HelloPacket& value)
	{
		
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, HelloPacket& outValue)
	{
		
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const CharacterInfo& value)
	{
		Serialize(stream, value.CharacterPrefabId);
		Serialize(stream, value.CurrentTile);
		Serialize(stream, value.NetworkId);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, CharacterInfo& outValue)
	{
		Deserialize(stream, outValue.CharacterPrefabId);
		Deserialize(stream, outValue.CurrentTile);
		Deserialize(stream, outValue.NetworkId);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const PlayerInfo& value)
	{
		Serialize(stream, value.Address);
		Serialize(stream, value.PlayerId);
		Serialize(stream, value.Character);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, PlayerInfo& outValue)
	{
		Deserialize(stream, outValue.Address);
		Deserialize(stream, outValue.PlayerId);
		Deserialize(stream, outValue.Character);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const WelcomePacket& value)
	{
		Serialize(stream, value.PlayerId);
		Serialize(stream, value.NetworkId);
		Serialize(stream, value.Players);
		Serialize(stream, value.Characters);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, WelcomePacket& outValue)
	{
		Deserialize(stream, outValue.PlayerId);
		Deserialize(stream, outValue.NetworkId);
		Deserialize(stream, outValue.Players);
		Deserialize(stream, outValue.Characters);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const IntroductionPacket& value)
	{
		Serialize(stream, value.PlayerId);
		Serialize(stream, value.Character);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, IntroductionPacket& outValue)
	{
		Deserialize(stream, outValue.PlayerId);
		Deserialize(stream, outValue.Character);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const DisconnectPacket& value)
	{

	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, DisconnectPacket& outValue)
	{

	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const PlayerMovePacket& value)
	{
		Serialize(stream, value.NetworkId);
		Serialize(stream, value.MoveToTile);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, PlayerMovePacket& outValue)
	{
		Deserialize(stream, outValue.NetworkId);
		Deserialize(stream, outValue.MoveToTile);
	}

}