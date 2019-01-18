#pragma once
#include "bltpch.h"
#include "Networking/Serialization.h"
#include "Networking/Deserialization.h"

namespace DND
{

	struct CharacterStats
	{
	public:
		int MaxHealth;
		int CurrentHealth;
		
		int Strength;
		int Dexterity;
		int Wisdom;
	};

	template<>
	inline void Serialize(OutputMemoryStream& stream, const CharacterStats& value)
	{
		Serialize(stream, value.MaxHealth);
		Serialize(stream, value.CurrentHealth);
		Serialize(stream, value.Strength);
		Serialize(stream, value.Dexterity);
		Serialize(stream, value.Wisdom);
	}

	template<>
	inline void Deserialize(InputMemoryStream& stream, CharacterStats& outValue)
	{
		Deserialize(stream, outValue.MaxHealth);
		Deserialize(stream, outValue.CurrentHealth);
		Deserialize(stream, outValue.Strength);
		Deserialize(stream, outValue.Dexterity);
		Deserialize(stream, outValue.Wisdom);
	}

	class UpdatedStatsEvent : public Event
	{
	public:
		const CharacterStats& Stats;
		GameObject* Object;

	public:
		UpdatedStatsEvent(const CharacterStats& stats, GameObject* object);

	};

	class StatsComponent : public Component
	{
	public:
		EventDispatcher<UpdatedStatsEvent> OnStatsChanged;

	private:
		CharacterStats m_Stats;

	public:
		StatsComponent(CharacterStats stats);

		inline const CharacterStats& Stats() const { return m_Stats; }
		void SetStats(const CharacterStats& stats);

		std::unique_ptr<Component> Clone() const override;

	};

}