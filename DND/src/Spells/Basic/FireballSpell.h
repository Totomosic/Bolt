#pragma once
#include "../Spell.h"
#include "../Dice.h"

namespace DND
{

	class FireballInstance : public SpellInstance
	{
	private:
		Vector3f m_Start;
		Vector3f m_Target;
		float m_Speed;
		DiceRollResult m_Damage;

	public:
		FireballInstance(const Vector3f& start, const Vector3f& target, float speed, const DiceRollResult& damage);

		SpellInstance::SpellCastResult Cast(GameObject* caster, const GameStateObjects& stateObjects) override;
		OutputMemoryStream SerializeInstance() const override;
	};

	class FireballSpell : public Spell
	{
	public:
		FireballSpell(int level);

		std::unique_ptr<SpellInstance> CreateInstance(InputMemoryStream& data) override;
		std::unique_ptr<SpellInstance> CreateInstance(GameObject* caster, const GameState& state) override;

	};

}