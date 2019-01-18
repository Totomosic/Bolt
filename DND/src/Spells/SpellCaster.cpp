#include "bltpch.h"
#include "SpellCaster.h"
#include "../GameManager.h"

#include "../Entities/TileMotion.h"

namespace DND
{

	SpellCaster::SpellCaster() : Component()
	{

	}

	void SpellCaster::Cast(id_t spellId, const OutputMemoryStream& castData)
	{
		SpellInfo& spell = GameManager::Get().Spells().GetSpell(spellId);
		BLT_CORE_WARN("CASTING SPELL {}", spell.Name);

		InputMemoryStream data(castData.GetRemainingDataSize());
		memcpy(data.GetBufferPtr(), castData.GetBufferPtr(), castData.GetRemainingDataSize());

		spell.CastFunc(gameObject(), data, GameManager::Get());
	}

	std::unique_ptr<Component> SpellCaster::Clone() const
	{
		return std::make_unique<SpellCaster>();
	}

}