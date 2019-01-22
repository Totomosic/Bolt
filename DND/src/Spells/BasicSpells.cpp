#include "bltpch.h"
#include "Dice.h"
#include "SpellList.h"
#include "../GameManager.h"
#include "../Entities/TileMotion.h"
#include "../Networking/NetworkIdentity.h"

#include "Animators/FireballAnimator.h"

#include "Basic/FireballSpell.h"

namespace DND
{

	void CreateBasicSpells(const ResourcePack& resources)
	{
		SpellList& spells = GameManager::Get().Spells();

		id_t explosionId = resources.GetResourceId("explosionAnimation");
		id_t fireballId = resources.GetResourceId("fireballAnimation");
		id_t fireballIcon = resources.GetResourceId("fireballIcon");

		id_t teleportIcon = resources.GetResourceId("teleportIcon");

		spells.AddSpell(std::make_unique<FireballSpell>(explosionId, fireballId));

	}

}