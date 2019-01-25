#pragma once
#include "bltpch.h"
#include "../../Networking/GamePlayPacketSerialization.h"

namespace DND
{

	class UpdatedStatsEvent : public Event
	{
	public:
		const CharacterStats& Stats;
		CharacterStats StatsDelta;
		GameObject* Object;

	public:
		UpdatedStatsEvent(const CharacterStats& stats, CharacterStats statsDelta, GameObject* object);

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