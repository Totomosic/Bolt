#pragma once
#include "SpellInstance.h"

namespace DND
{

	struct GameState;

	class Spell
	{
	public:
		struct SubSpell
		{
		public:
			std::unique_ptr<Spell> spell;
		};

	private:
		blt::string m_Name;
		blt::string m_Desc;
		id_t m_Id;

		std::vector<SubSpell> m_SubSpells;

	public:
		Spell();

		const blt::string& Name() const;
		const blt::string& Desc() const;
		id_t Id() const;

		virtual std::unique_ptr<SpellInstance> CreateInstance(GameObject* caster, const GameState& state);

	};

}