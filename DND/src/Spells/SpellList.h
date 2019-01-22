#pragma once
#include "bltpch.h"
#include "Spell.h"

namespace DND
{

	class SpellList
	{
	public:
		static constexpr size_t MAX_SPELLS = 100;

	private:
		std::unique_ptr<Spell> m_Spells[MAX_SPELLS];
		int m_SpellCount;

	public:
		SpellList();

		int SpellCount() const;
		Spell* GetSpell(id_t id) const;

		Spell& AddSpell(std::unique_ptr<Spell>&& spell);

	};

}