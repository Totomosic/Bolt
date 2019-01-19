#pragma once
#include "bltpch.h"

namespace DND
{

	class GameManager;

	struct SpellInfo
	{
	public:
		using CreateSpellFunc = std::function<OutputMemoryStream(GameObject*, GameManager&)>;
		using CastSpellFunc = std::function<void(GameObject*, InputMemoryStream&, GameManager&)>;

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