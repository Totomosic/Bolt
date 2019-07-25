#include "bltpch.h"
#include "StatsComponent.h"

namespace DND
{

	StatsComponent::StatsComponent(const StatsNetworkData& maxStats) : StatsComponent(maxStats, maxStats)
	{
	
	}

	StatsComponent::StatsComponent(const StatsNetworkData& maxStats, const StatsNetworkData& currentStats) : Component(),
		m_MaxStats(maxStats), m_CurrentStats(currentStats)
	{
		
	}

	const StatsNetworkData& StatsComponent::MaxStats() const
	{
		return m_MaxStats;
	}

	const StatsNetworkData& StatsComponent::CurrentStats() const
	{
		return m_CurrentStats;
	}

	StatsNetworkData StatsComponent::StatsDelta() const
	{
		return CurrentStats() - MaxStats();
	}

	void StatsComponent::SetMaxStats(const StatsNetworkData& stats)
	{
		m_MaxStats = stats;
	}

	void StatsComponent::SetCurrentStats(const StatsNetworkData& stats)
	{
		m_CurrentStats = stats;
	}

	void StatsComponent::AddMaxStatsDelta(const StatsNetworkData& delta)
	{
		m_MaxStats = m_MaxStats + delta;
	}

	void StatsComponent::AddCurrentStatsDelta(const StatsNetworkData& delta)
	{
		m_CurrentStats = m_CurrentStats + delta;
	}

	std::unique_ptr<Component> StatsComponent::Clone() const
	{
		return std::make_unique<StatsComponent>(MaxStats(), CurrentStats());
	}
}