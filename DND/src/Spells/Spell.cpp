#include "bltpch.h"
#include "Spell.h"

namespace DND
{

	Spell::Spell()
	{
		
	}

	Spell::Spell(const blt::string& name, const blt::string& desc, int level)
		: m_Name(name), m_Desc(desc), m_Id(-1), m_Level(level), m_SubSpells()
	{
	
	}

	const blt::string& Spell::Name() const
	{
		return m_Name;
	}

	const blt::string& Spell::Desc() const
	{
		return m_Desc;
	}

	id_t Spell::Id() const
	{
		return m_Id;
	}

	int Spell::Level() const
	{
		return m_Level;
	}

	void Spell::SetId(id_t id)
	{
		m_Id = id;
	}

}