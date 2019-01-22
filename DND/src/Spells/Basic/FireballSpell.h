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

		SpellInstance::SpellCastResult Cast(Spell* spell, GameObject* caster, const GameStateObjects& stateObjects) override;
		OutputMemoryStream SerializeInstance() const override;
	};

	class FireballSpell : public Spell
	{
	public:
		id_t m_ExplosionAnimation;
		id_t m_FireballAnimation;

	public:
		FireballSpell(id_t explosionId, id_t fireballId);

		std::unique_ptr<SpellInstance> CreateInstance(InputMemoryStream& data) override;
		std::unique_ptr<SpellInstance> CreateInstance(GameObject* caster, const GameState& state) override;

	};

}