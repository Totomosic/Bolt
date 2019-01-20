#pragma once
#include "bltpch.h"

namespace DND
{

	class GameManager;
	struct GameState;
	struct GameStateObjects;

	struct SpellInfo
	{
	public:
		using CreateSpellFunc = std::function<OutputMemoryStream(GameObject*, const GameState&)>;
		using CastSpellFunc = std::function<void(GameObject*, InputMemoryStream&, const GameStateObjects&)>;

	public:
		blt::string Name;
		blt::string Desc;
		float CastTime;
		float Cooldown;
		int Level;
		id_t SpellIconId;
		CreateSpellFunc CreateFunc;
		CastSpellFunc CastFunc;
		id_t SpellId;
	};

}