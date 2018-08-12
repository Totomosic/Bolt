#include "RandomDistribution.h"

namespace Bolt
{

	RandomDistribution::RandomDistribution() : RandomDistribution((size_t)time(nullptr))
	{
	
	}

	RandomDistribution::RandomDistribution(size_t seed)
		: m_Generator(seed)
	{
		
	}

	void RandomDistribution::SetSeed(size_t seed)
	{
		m_Generator.seed(seed);
	}

	int RandomDistribution::NextInt(int min, int max) const
	{
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(m_Generator);
	}

	int RandomDistribution::NextInt(int max) const
	{
		return NextInt(0, max);
	}

	int RandomDistribution::NextInt() const
	{
		return NextInt(0, 1);
	}

	float RandomDistribution::NextFloat(float min, float max) const
	{
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(m_Generator);
	}

	float RandomDistribution::NextFloat(float max) const
	{
		return NextFloat(0.0f, max);
	}

	float RandomDistribution::NextFloat() const
	{
		return NextFloat(0.0f, 1.0f);
	}

}