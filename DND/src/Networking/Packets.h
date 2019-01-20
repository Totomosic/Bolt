#pragma once
#include "bltpch.h"
#include "../Map/Tilemap.h"

#include "Serialization.h"
#include "Deserialization.h"
#include "Entities/Characters/CharacterStats.h"

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
		CastSpell,
		StatUpdate,
		Death,
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
		CharacterStats Stats;
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
		SocketAddress Address;
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

	struct CastSpellPacket
	{
	public:
		id_t PlayerId;
		id_t CasterNetworkId;
		id_t SpellId;
		OutputMemoryStream SpellData;
	};

	struct StatUpdatePacket
	{
	public:
		id_t NetworkId;
		CharacterStats Stats;
	};
	
	struct DeathPacket
	{
	public:
		id_t NetworkId;
		id_t PlayerId; // Should be set to InvalidID if not a player
		id_t KillerId;
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
	inline PacketType TypeOfPacket<CastSpellPacket>()
	{
		return PacketType::CastSpell;
	}
	template<>
	inline PacketType TypeOfPacket<StatUpdatePacket>()
	{
		return PacketType::StatUpdate;
	}
	template<>
	inline PacketType TypeOfPacket<DeathPacket>()
	{
		return PacketType::Death;
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
		Serialize(stream, value.Stats);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, CharacterInfo& outValue)
	{
		Deserialize(stream, outValue.CharacterPrefabId);
		Deserialize(stream, outValue.CurrentTile);
		Deserialize(stream, outValue.NetworkId);
		Deserialize(stream, outValue.Stats);
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
		Serialize(stream, value.Address);
		Serialize(stream, value.Players);
		Serialize(stream, value.Characters);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, WelcomePacket& outValue)
	{
		Deserialize(stream, outValue.PlayerId);
		Deserialize(stream, outValue.NetworkId);
		Deserialize(stream, outValue.Address);
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

	template<>
	inline void Serialize(OutputMemoryStream& stream, const CastSpellPacket& value)
	{
		Serialize(stream, value.PlayerId);
		Serialize(stream, value.CasterNetworkId);
		Serialize(stream, value.SpellId);
		Serialize(stream, value.SpellData);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, CastSpellPacket& outValue)
	{
		Deserialize(stream, outValue.PlayerId);
		Deserialize(stream, outValue.CasterNetworkId);
		Deserialize(stream, outValue.SpellId);
		Deserialize(stream, outValue.SpellData);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const StatUpdatePacket& value)
	{
		Serialize(stream, value.NetworkId);
		Serialize(stream, value.Stats);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, StatUpdatePacket& outValue)
	{
		Deserialize(stream, outValue.NetworkId);
		Deserialize(stream, outValue.Stats);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const DeathPacket& value)
	{
		Serialize(stream, value.NetworkId);
		Serialize(stream, value.PlayerId);
		Serialize(stream, value.KillerId);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, DeathPacket& outValue)
	{
		Deserialize(stream, outValue.NetworkId);
		Deserialize(stream, outValue.PlayerId);
		Deserialize(stream, outValue.KillerId);
	}

}