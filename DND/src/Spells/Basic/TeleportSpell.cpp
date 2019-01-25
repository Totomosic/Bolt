#include "bltpch.h"
#include "TeleportSpell.h"
#include "../../GameManager.h"

#include "../../Entities/TileTransform.h"
#include "../../Entities/TileMotion.h"
#include "../../Entities/SpriteAnimator.h"

namespace DND
{

	TeleportInstance::TeleportInstance(Tile location) : SpellInstance(),
		m_Location(std::move(location))
	{
		
	}

	SpellInstance::SpellCastResult TeleportInstance::Cast(GameObject* caster, const GameStateObjects& state)
	{
		GameManager::Get().AddActiveTimer(&Time::RenderingTimeline().AddFunction(0.5f, [caster, location = m_Location]()
		{
			caster->Components().GetComponent<TileTransform>().SetCurrentTile(location, true);
			caster->Components().GetComponent<TileMotion>().SetTargetTile(location);
		}));

		caster->Components().GetComponent<SpriteAnimator>().PlayAnimation(state.Animations->SwordsmanForwardTeleport, 1.0f);

		SpellCastResult result;
		result.Cooldown = 5;
		result.CastTime = 1;
		result.Type = CastType::Action;
		return result;
	}

	OutputMemoryStream TeleportInstance::SerializeInstance() const
	{
		OutputMemoryStream result;
		Serialize(result, m_Location);
		return result;
	}

	TeleportSpell::TeleportSpell(int level) : Spell("Teleport", "", level)
	{
	
	}

	std::unique_ptr<SpellInstance> TeleportSpell::CreateInstance(InputMemoryStream& stream)
	{
		Tile location;
		Deserialize(stream, location);
		return std::make_unique<TeleportInstance>(location);
	}

	std::unique_ptr<SpellInstance> TeleportSpell::CreateInstance(GameObject* caster, const GameState& state)
	{
		return std::make_unique<TeleportInstance>(state.SelectedTile);
	}

}