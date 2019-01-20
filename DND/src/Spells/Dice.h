#pragma once
#include "bltpch.h"
#include "../Networking/Serialization.h"
#include "../Networking/Deserialization.h"

namespace DND
{

	struct DiceRoll
	{
	public:
		int MaxRoll;
		int Result;
	};

	struct DiceRollResult
	{
	public:
		int Result;
		std::vector<DiceRoll> DiceRolls;
		int Modifier;

	public:
		inline blt::string ToString() const
		{
			blt::string str = std::to_string(Result);
			if (DiceRolls.size() > 0)
			{
				str += " (" + std::to_string(DiceRolls[0].Result);
				for (int i = 1; i < DiceRolls.size(); i++)
				{
					str += "+" + std::to_string(DiceRolls[i].Result);
				}
				str += ")";
				if (Modifier > 0)
				{
					str += "+" + std::to_string(Modifier);
				}
			}
			return str;
		}

	};

	class Dice
	{
	public:
		Dice() = delete;

		static DiceRollResult Roll(int diceCount, int diceFaces, int modifier);
		static DiceRollResult Roll(int diceFaces, int modifier);
	};

	template<>
	inline void Serialize(OutputMemoryStream& stream, const DiceRoll& value)
	{
		Serialize(stream, value.MaxRoll);
		Serialize(stream, value.Result);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, DiceRoll& outValue)
	{
		Deserialize(stream, outValue.MaxRoll);
		Deserialize(stream, outValue.Result);
	}

	template<>
	inline void Serialize(OutputMemoryStream& stream, const DiceRollResult& value)
	{
		Serialize(stream, value.Result);
		Serialize(stream, value.DiceRolls);
		Serialize(stream, value.Modifier);
	}
	template<>
	inline void Deserialize(InputMemoryStream& stream, DiceRollResult& outValue)
	{
		Deserialize(stream, outValue.Result);
		Deserialize(stream, outValue.DiceRolls);
		Deserialize(stream, outValue.Modifier);
	}

}