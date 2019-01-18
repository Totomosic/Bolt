#include "bltpch.h"
#include "SpellList.h"

namespace DND
{
	
	SpellList::SpellList()
		: m_Spells(), m_SpellCount(0)
	{
		
	}

	int SpellList::SpellCount() const
	{
		return m_SpellCount;
	}

	const SpellInfo& SpellList::GetSpell(id_t id) const
	{
		return m_Spells[id];
	}

	SpellInfo& SpellList::GetSpell(id_t id)
	{
		return m_Spells[id];
	}

	SpellInfo& SpellList::AddSpell(const SpellInfo& spell)
	{
		id_t index = m_SpellCount;
		SpellInfo& info = m_Spells[index];
		info = spell;
		return info;
	}

}