#pragma once
#include "Random.h"

namespace Bolt
{

	struct BLT_API RandomDistribution
	{
	private:
		mutable std::minstd_rand m_Generator;

	public:
		RandomDistribution();
		RandomDistribution(uint32_t seed);

		void SetSeed(uint32_t seed);

		// Generates random integer [min, max]
		int NextInt(int min, int max) const;
		// Generates random integer [0, max]
		int NextInt(int max) const;
		// Generates random integer [0, 1]
		int NextInt() const;

		// Generates random floating point [min, max)
		float NextFloat(float min, float max) const;
		// Generates random floating point [0, max)
		float NextFloat(float max) const;
		// Generates random floating point [0, 1)
		float NextFloat() const;

	};

}