#pragma once
#include "Spell.h"

namespace RPG
{

	class MeteorSpell : public Spell
	{
	private:
		float m_BaseDamage;
		float m_Scaling;
		float m_OuterDamageProp;
		Vector2f m_Size;
		const Texture2D* m_Texture;

	public:
		MeteorSpell(float cooldown, float cost, float baseDamage, float damageScaling, float outerScaling, const Vector2f& size, const Texture2D* texture);

		void Cast(Character* caster, const MapTile* selectedTile, const Character* selectedCharacter) override;

	};

}