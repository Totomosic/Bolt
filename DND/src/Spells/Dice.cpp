#include "bltpch.h"
#include "Dice.h"

namespace DND
{

	DiceRollResult Dice::Roll(int diceCount, int diceFaces, int modifier)
	{
		DiceRollResult result;
		result.Result = modifier;
		result.Modifier = modifier;
		for (int i = 0; i < diceCount; i++)
		{
			int roll = Random::NextInt(1, diceFaces);
			result.Result += roll;
			result.DiceRolls.push_back({ diceFaces, roll });
		}
		return result;
	}

	DiceRollResult Dice::Roll(int diceFaces, int modifier)
	{
		return Roll(1, diceFaces, modifier);
	}

}