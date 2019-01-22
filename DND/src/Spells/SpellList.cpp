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

	Spell* SpellList::GetSpell(id_t id) const
	{
		return m_Spells[id].get();
	}

	Spell& SpellList::AddSpell(std::unique_ptr<Spell>&& spell)
	{
		id_t index = m_SpellCount;
		std::unique_ptr<Spell>& info = m_Spells[index];
		info = std::move(spell);
		info->SetId(index);
		m_SpellCount++;
		return *info;
	}

}