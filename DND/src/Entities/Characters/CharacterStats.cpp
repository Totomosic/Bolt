#include "bltpch.h"
#include "CharacterStats.h"
#include "../../Events.h"

namespace DND
{

	UpdatedStatsEvent::UpdatedStatsEvent(const CharacterStats& stats, GameObject* object) : Event(),
		Stats(stats), Object(object)
	{
	
	}

	StatsComponent::StatsComponent(CharacterStats stats) : Component(),
		OnStatsChanged(STATS_CHANGED_EVENT), m_Stats(std::move(stats))
	{
	
	}

	void StatsComponent::SetStats(const CharacterStats& stats)
	{
		m_Stats = stats;
		OnStatsChanged.Post(std::make_unique<UpdatedStatsEvent>(m_Stats, gameObject()));
	}

	std::unique_ptr<Component> StatsComponent::Clone() const
	{
		return std::make_unique<StatsComponent>(m_Stats);
	}

}