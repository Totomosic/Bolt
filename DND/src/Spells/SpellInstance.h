#pragma once
#include "bltpch.h"

namespace DND
{

	struct GameStateObjects;
	class Spell;

	class SpellInstance
	{
	public:
		struct SpellCastResult
		{
		public:
			float Cooldown;
		};

	public:
		virtual SpellCastResult Cast(Spell* spell, GameObject* caster, const GameStateObjects& stateObjects) = 0;
		virtual OutputMemoryStream SerializeInstance() const = 0;

	};

}