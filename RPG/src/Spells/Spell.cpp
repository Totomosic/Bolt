#include "Spell.h"

namespace RPG
{

	Spell::Spell(float cooldown, float cost)
		: m_Cooldown(cooldown), m_Cost(cost)
	{
	
	}

	float Spell::Cooldown() const
	{
		return m_Cooldown;
	}

	float Spell::Cost() const
	{
		return m_Cost;
	}

	void Spell::SetCooldown(float cooldown)
	{
		m_Cooldown = cooldown;
	}

	void Spell::SetCost(float cost)
	{
		m_Cost = cost;
	}

}