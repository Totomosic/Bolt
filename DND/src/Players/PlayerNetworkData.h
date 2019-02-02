#pragma once
#include "Characters/Entities/EntityNetworkData.h"
#include "../Network/AddressPair.h"

namespace DND
{

	struct PlayerNetworkData
	{
	public:
		AddressPair Address;
		id_t PlayerId;
		EntityNetworkData PlayerObject;

	};

	template<>
	inline void Serialize(OutputMemoryStream& stream, const PlayerNetworkData& value)
	{
		Serialize(stream, value.Address);
		Serialize(stream, value.PlayerId);
		Serialize(stream, value.PlayerObject);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, PlayerNetworkData& value)
	{
		Deserialize(stream, value.Address);
		Deserialize(stream, value.PlayerId);
		Deserialize(stream, value.PlayerObject);
	}

}