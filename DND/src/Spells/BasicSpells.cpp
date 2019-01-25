#include "bltpch.h"
#include "../GameManager.h"
#include "SpellList.h"

#include "Basic/TeleportSpell.h"
#include "Basic/FireballSpell.h"

namespace DND
{

	void CreateBasicSpells(const ResourcePack& resources)
	{
		SpellList& spells = GameManager::Get().Spells();

		spells.AddSpell(std::make_unique<TeleportSpell>(2));
		spells.AddSpell(std::make_unique<FireballSpell>(3));

	}

}