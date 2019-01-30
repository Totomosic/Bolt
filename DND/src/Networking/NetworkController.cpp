#include "bltpch.h"
#include "NetworkController.h"
#include "../GameManager.h"
#include "../Entities/Characters/CharacterStats.h"
#include "../Entities/TileMotion.h"
#include "../Spells/SpellCaster.h"

namespace DND
{

	NetworkController::NetworkController() : Component()
	{
		
	}

	void NetworkController::MoveToTile(const Tile& tile) const
	{
		gameObject()->Components().GetComponent<TileMotion>().SetTargetTile(tile);
	}

	void NetworkController::CastSpell(id_t spellId, const OutputMemoryStream& castData) const
	{
		gameObject()->Components().GetComponent<SpellCaster>().Cast(spellId, castData, GameManager::Get().GetStateObjects());
	}

	void NetworkController::UpdateStats(const CharacterStats& newStats) const
	{
		gameObject()->Components().GetComponent<StatsComponent>().SetStats(newStats);
	}

	std::unique_ptr<Component> NetworkController::Clone() const
	{
		return std::make_unique<NetworkController>();
	}

}