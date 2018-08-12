#pragma once
#include "Spell.h"

namespace RPG
{

	class Character;

	class SpellManager
	{
	private:
		Character* m_Owner;
		std::vector<std::unique_ptr<Spell>> m_Spells;

	public:
		SpellManager(Character* owner);

		Character* Owner() const;
		const std::vector<std::unique_ptr<Spell>>& Spells() const;
		int SpellCount() const;
		Spell* GetSpell(int index) const;

		template<typename SpellType>
		SpellType* GetSpell(int index) const
		{
			return (SpellType*)GetSpell(index);
		}

		void SetOwner(Character* owner);
		Spell* AddSpell(std::unique_ptr<Spell>&& spell);
		void RemoveSpell(int index);
		void RemoveSpell(const Spell* spell);

		bool Cast(int index) const;
		bool Cast(Spell* spell) const;

	};

}