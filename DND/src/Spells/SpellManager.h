#pragma once
#include "Spell.h"

namespace DND
{

	class SpellManager
	{
	public:
		struct SpellData
		{
		public:
			Spell* spell;
			float Cooldown;
			float MaxCooldown;
		};

	private:
		std::vector<SpellData> m_Spells;

	public:
		SpellManager();

		bool CanCast(id_t index) const;
		Spell* GetSpell(id_t index) const;
		id_t AddSpell(id_t spellId);

		SpellInstance::SpellCastResult CastSpell(id_t index, const std::unique_ptr<SpellInstance>& instance, GameObject* caster, const GameStateObjects& state);

		void Update();

	};

}