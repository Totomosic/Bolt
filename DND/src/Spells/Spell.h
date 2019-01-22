#pragma once
#include "SpellInstance.h"
#include "../Networking/Serialization.h"
#include "../Networking/Deserialization.h"

namespace DND
{

	struct GameState;

	class Spell
	{
	public:
		struct SubSpell
		{
		public:
			std::unique_ptr<Spell> spell;
		};

	private:
		blt::string m_Name;
		blt::string m_Desc;
		id_t m_Id;
		int m_Level;

		std::vector<SubSpell> m_SubSpells;

	public:
		Spell();
		Spell(const blt::string& name, const blt::string& desc, int level);

		const blt::string& Name() const;
		const blt::string& Desc() const;
		id_t Id() const;
		int Level() const;

		virtual std::unique_ptr<SpellInstance> CreateInstance(InputMemoryStream& data) = 0;
		virtual std::unique_ptr<SpellInstance> CreateInstance(GameObject* caster, const GameState& state) = 0;

		friend class SpellList;

	private:
		void SetId(id_t id);

	};

}