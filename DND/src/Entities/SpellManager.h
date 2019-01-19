#pragma once
#include "bltpch.h"
#include "../Spells/SpellInfo.h"
#include "Characters/CharacterStats.h"

namespace DND
{

	class GameManager;

	class SpellCastEvent : public Event
	{
	public:
		id_t SpellIndex;
		id_t SpellId;
		float Cooldown;
	};

	class SpellManager
	{
	public:
		struct SpellCastData
		{
		public:
			id_t SpellId = (id_t)-1;
			float Cooldown = 0;
			float CurrentTime = 0;
		};
		
	public:
		EventDispatcher<SpellCastEvent> OnSpellCast;

	private:
		id_t m_MaxSpells;
		std::unique_ptr<SpellCastData[]> m_Spells;
		std::unordered_map<Keycode, id_t> m_SpellKeyMap;
		std::vector<Keycode> m_MappedKeys;

	public:
		SpellManager(int maxSpells);

		const std::vector<Keycode>& MappedKeys() const;

		id_t MaxSpells() const;
		id_t GetSpellIndex(Keycode key) const;
		id_t GetSpellId(id_t spellIndex) const;
		bool HasSpell(id_t spellIndex) const;
		bool CanCast(id_t spellIndex) const;
		float CooldownOf(id_t spellIndex) const;
		float RemainingCooldownOf(id_t spellIndex) const;

		void SetSpell(id_t spellIndex, id_t spellId);
		void MapKeyToSpell(Keycode key, id_t spellIndex);
		void CastSpell(id_t spellIndex);

		void Update(float deltaTime);

	};

}