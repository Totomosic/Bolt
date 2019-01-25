#pragma once
#include "../Spell.h"

namespace DND
{

	class TeleportInstance : public SpellInstance
	{
	private:
		Tile m_Location;

	public:
		TeleportInstance(Tile location);

		SpellInstance::SpellCastResult Cast(GameObject* caster, const GameStateObjects& state) override;
		OutputMemoryStream SerializeInstance() const override;
	};

	class TeleportSpell : public Spell
	{
	public:
		TeleportSpell(int level);

		std::unique_ptr<SpellInstance> CreateInstance(InputMemoryStream& data) override;
		std::unique_ptr<SpellInstance> CreateInstance(GameObject* caster, const GameState& state) override;

	};

}