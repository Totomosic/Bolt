#include "bltpch.h"
#include "SpellManager.h"
#include "../GameManager.h"

namespace DND
{

	SpellManager::SpellManager(int maxSpells)
		: OnSpellCast(SPELL_CAST_EVENT), m_MaxSpells(maxSpells), m_Spells(std::make_unique<SpellManager::SpellCastData[]>(maxSpells)), m_SpellKeyMap(), m_MappedKeys()
	{
		
	}

	const std::vector<Keycode>& SpellManager::MappedKeys() const
	{
		return m_MappedKeys;
	}

	id_t SpellManager::MaxSpells() const
	{
		return m_MaxSpells;
	}

	id_t SpellManager::GetSpellIndex(Keycode key) const
	{
		return m_SpellKeyMap.at(key);
	}

	id_t SpellManager::GetSpellId(id_t spellIndex) const
	{
		return m_Spells[spellIndex].SpellId;
	}

	bool SpellManager::HasSpell(id_t index) const
	{
		return m_Spells[index].SpellId != (id_t)-1;
	}

	bool SpellManager::CanCast(id_t index) const
	{
		return m_Spells[index].CurrentTime == m_Spells[index].Cooldown;
	}

	float SpellManager::CooldownOf(id_t index) const
	{
		return m_Spells[index].Cooldown;
	}

	float SpellManager::RemainingCooldownOf(id_t index) const
	{
		return m_Spells[index].Cooldown - m_Spells[index].CurrentTime;
	}

	void SpellManager::SetSpell(id_t index, id_t spellId)
	{
		SpellInfo& info = GameManager::Get().Spells().GetSpell(spellId);
		m_Spells[index] = { spellId, info.Cooldown, info.Cooldown };
	}

	void SpellManager::MapKeyToSpell(Keycode key, id_t index)
	{
		if (m_SpellKeyMap.find(key) != m_SpellKeyMap.end())
		{
			auto it = std::find(m_MappedKeys.begin(), m_MappedKeys.end(), key);
			if (it != m_MappedKeys.end())
			{
				m_MappedKeys.erase(it);
			}
		}
		m_SpellKeyMap[key] = index;
		m_MappedKeys.push_back(key);
	}

	void SpellManager::CastSpell(id_t spellIndex)
	{
		SpellManager::SpellCastData& data = m_Spells[spellIndex];
		data.CurrentTime = 0;
		std::unique_ptr<SpellCastEvent> e = std::make_unique<SpellCastEvent>();
		e->SpellIndex = spellIndex;
		e->SpellId = data.SpellId;
		e->Cooldown = data.Cooldown;
		OnSpellCast.Post(std::move(e));
	}

	void SpellManager::Update(float deltaTime)
	{
		for (int i = 0; i < (int)m_MaxSpells; i++)
		{
			SpellManager::SpellCastData& data = m_Spells[i];
			if (data.CurrentTime != data.Cooldown)
			{
				data.CurrentTime += std::min(deltaTime, data.Cooldown - data.CurrentTime);
			}
		}
	}

}