#include "SpellManager.h"
#include "..\GameManager.h"

namespace RPG
{

	SpellManager::SpellManager(Character* owner)
		: m_Owner(owner), m_Spells()
	{
	
	}

	Character* SpellManager::Owner() const
	{
		return m_Owner;
	}

	const std::vector<std::unique_ptr<Spell>>& SpellManager::Spells() const
	{
		return m_Spells;
	}

	int SpellManager::SpellCount() const
	{
		return m_Spells.size();
	}

	Spell* SpellManager::GetSpell(int index) const
	{
		return m_Spells[index].get();
	}

	void SpellManager::SetOwner(Character* owner)
	{
		m_Owner = owner;
	}
	
	Spell* SpellManager::AddSpell(std::unique_ptr<Spell>&& spell)
	{
		Spell* ptr = spell.get();
		m_Spells.push_back(std::move(spell));
		return ptr;
	}

	void SpellManager::RemoveSpell(int index)
	{
		m_Spells.erase(m_Spells.begin() + index);
	}

	void SpellManager::RemoveSpell(const Spell* spell)
	{
		auto it = std::find_if(m_Spells.begin(), m_Spells.end(), [spell](const std::unique_ptr<Spell>& ptr)
		{
			return spell == ptr.get();
		});
		if (it != m_Spells.end())
		{
			m_Spells.erase(it);
		}
	}

	bool SpellManager::Cast(int index) const
	{
		return Cast(GetSpell(index));
	}

	bool SpellManager::Cast(Spell* spell) const
	{
		spell->Cast(Owner(), GameManager::SelectedTile, GameManager::SelectedCharacter);
		return true;
	}

}