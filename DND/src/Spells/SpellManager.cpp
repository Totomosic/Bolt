#include "bltpch.h"
#include "SpellManager.h"

#include "../GameManager.h"

namespace DND
{

	SpellManager::SpellManager()
	{

	}

	bool SpellManager::CanCast(id_t index) const
	{
		return m_Spells[index].Cooldown == 0;
	}

	Spell* SpellManager::GetSpell(id_t index) const
	{
		return m_Spells[index].spell;
	}

	id_t SpellManager::AddSpell(id_t spellId)
	{
		id_t index = m_Spells.size();
		m_Spells.push_back({ GameManager::Get().Spells().GetSpell(spellId) });
		return index;
	}

	SpellInstance::SpellCastResult SpellManager::CastSpell(id_t index, const std::unique_ptr<SpellInstance>& instance, GameObject* caster, const GameStateObjects& state)
	{
		SpellInstance::SpellCastResult result = instance->Cast(caster, state);
		m_Spells[index].Cooldown = result.Cooldown;
		m_Spells[index].MaxCooldown = result.Cooldown;
		CastSpellPacket packet;
		packet.NetworkId = caster->Components().GetComponent<NetworkIdentity>().NetworkId;
		packet.SpellId = GetSpell(index)->Id();
		packet.SpellData = instance->SerializeInstance();
		GameManager::Get().Network().SendPacketToAll(packet);
		return result;
	}

	void SpellManager::Update()
	{
		for (SpellData& data : m_Spells)
		{
			if (data.Cooldown != 0)
			{
				data.Cooldown -= std::min(data.Cooldown, (float)Time::RenderingTimeline().DeltaTime());
			}
		}
	}

}