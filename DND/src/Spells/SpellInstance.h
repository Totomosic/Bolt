#pragma once
#include "bltpch.h"

namespace DND
{

	struct GameStateObjects;

	enum class CastType
	{
		Action,
		BonusAction
	};

	class SpellInstance
	{
	public:
		struct SpellCastResult
		{
		public:
			float Cooldown;
			float CastTime;
			CastType Type;
		};

	public:
		virtual SpellCastResult Cast(GameObject* caster, const GameStateObjects& stateObjects) = 0;
		virtual OutputMemoryStream SerializeInstance() const = 0;

	};

}