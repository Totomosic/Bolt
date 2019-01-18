#pragma once
#include "bltpch.h"
#include "SpellInfo.h"

namespace DND
{

	class SpellList
	{
	public:
		static constexpr size_t MAX_SPELLS = 100;

	private:
		SpellInfo m_Spells[MAX_SPELLS];
		int m_SpellCount;

	public:
		SpellList();

		int SpellCount() const;
		const SpellInfo& GetSpell(id_t id) const;
		SpellInfo& GetSpell(id_t id);

		SpellInfo& AddSpell(const SpellInfo& spell);

	};

}