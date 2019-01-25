#include "bltpch.h"
#include "CharacterStats.h"
#include "../../Events.h"

namespace DND
{

	UpdatedStatsEvent::UpdatedStatsEvent(const CharacterStats& stats, CharacterStats statsDelta, GameObject* object) : Event(),
		Stats(stats), StatsDelta(statsDelta), Object(object)
	{
	
	}

	StatsComponent::StatsComponent(CharacterStats stats) : Component(),
		OnStatsChanged(STATS_CHANGED_EVENT), m_Stats(std::move(stats))
	{
	
	}

	void StatsComponent::SetStats(const CharacterStats& stats)
	{
		CharacterStats delta = CharacterStats::GetDelta(stats, m_Stats);
		m_Stats = stats;
		OnStatsChanged.Post(std::make_unique<UpdatedStatsEvent>(m_Stats, delta, gameObject()));
	}

	std::unique_ptr<Component> StatsComponent::Clone() const
	{
		return std::make_unique<StatsComponent>(m_Stats);
	}

}