#pragma once
#include "BoltEngine.h"

using namespace Bolt;

namespace RPG
{

	class Character;
	class MapTile;

	class Spell
	{
	protected:
		float m_Cooldown;
		float m_Cost;

	public:
		Spell(float cooldown, float cost);

		float Cooldown() const;
		float Cost() const;

		void SetCooldown(float cooldown);
		void SetCost(float cost);

		virtual void Cast(Character* caster, const MapTile* selectedTile, const Character* selectedCharacter) = 0;

	};

}