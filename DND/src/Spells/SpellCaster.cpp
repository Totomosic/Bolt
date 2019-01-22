#include "bltpch.h"
#include "SpellCaster.h"
#include "../GameManager.h"

#include "../Entities/TileMotion.h"

namespace DND
{

	SpellCaster::SpellCaster() : Component()
	{

	}

	void SpellCaster::Cast(id_t spellId, const OutputMemoryStream& castData, const GameStateObjects& state)
	{
		Spell* spell = GameManager::Get().Spells().GetSpell(spellId);

		InputMemoryStream data(castData.GetRemainingDataSize());
		memcpy(data.GetBufferPtr(), castData.GetBufferPtr(), castData.GetRemainingDataSize());

		std::unique_ptr<SpellInstance> instance = spell->CreateInstance(data);
		instance->Cast(spell, gameObject(), state);
	}

	std::unique_ptr<Component> SpellCaster::Clone() const
	{
		return std::make_unique<SpellCaster>();
	}

}