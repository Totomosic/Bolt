#pragma once
#include "EntityStats.h"

namespace DND
{

	class StatsComponent : public Component
	{
	private:
		StatsNetworkData m_MaxStats;
		StatsNetworkData m_CurrentStats;

	public:
		StatsComponent(const StatsNetworkData& maxStats);
		StatsComponent(const StatsNetworkData& maxStats, const StatsNetworkData& currentStats);

		const StatsNetworkData& MaxStats() const;
		const StatsNetworkData& CurrentStats() const;
		StatsNetworkData StatsDelta() const;

		void SetMaxStats(const StatsNetworkData& stats);
		void SetCurrentStats(const StatsNetworkData& stats);
		void AddCurrentStatsDelta(const StatsNetworkData& statsDelta);
		void AddMaxStatsDelta(const StatsNetworkData& statsDelta);

		std::unique_ptr<Component> Clone() const override;

	};

}