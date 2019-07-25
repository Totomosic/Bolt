#pragma once
#include "PlayerNetworkData.h"

namespace DND
{

	struct GameNetworkData
	{
	public:
		std::vector<PlayerNetworkData> Players;
	};

	template<>
	inline void Serialize(OutputMemoryStream& stream, const GameNetworkData& value)
	{
		Serialize(stream, value.Players);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, GameNetworkData& value)
	{
		Deserialize(stream, value.Players);
	}

}