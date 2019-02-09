#pragma once
#include "../../../Network/Serialization.h"
#include "../../../Network/Deserialization.h"

namespace DND
{

	struct StatsNetworkData
	{
	public:
		int Strength;
		int Dexterity;
		int Constitution;
		int Intelligence;
		int Wisdom;
		int Charisma;
	};

	inline StatsNetworkData operator+(const StatsNetworkData& left, const StatsNetworkData& right)
	{
		StatsNetworkData result = left;
		result.Strength += right.Strength;
		result.Dexterity += right.Dexterity;
		result.Constitution += right.Constitution;
		result.Intelligence += right.Intelligence;
		result.Wisdom += right.Wisdom;
		result.Charisma += right.Charisma;
		return result;
	}

	inline StatsNetworkData operator-(const StatsNetworkData& left, const StatsNetworkData& right)
	{
		StatsNetworkData result = left;
		result.Strength -= right.Strength;
		result.Dexterity -= right.Dexterity;
		result.Constitution -= right.Constitution;
		result.Intelligence -= right.Intelligence;
		result.Wisdom -= right.Wisdom;
		result.Charisma -= right.Charisma;
		return result;
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const StatsNetworkData& value)
	{
		Serialize(stream, value.Strength);
		Serialize(stream, value.Dexterity);
		Serialize(stream, value.Constitution);
		Serialize(stream, value.Intelligence);
		Serialize(stream, value.Wisdom);
		Serialize(stream, value.Charisma);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, StatsNetworkData& value)
	{
		Deserialize(stream, value.Strength);
		Deserialize(stream, value.Dexterity);
		Deserialize(stream, value.Constitution);
		Deserialize(stream, value.Intelligence);
		Deserialize(stream, value.Wisdom);
		Deserialize(stream, value.Charisma);
	}

}