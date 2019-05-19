#pragma once
#include "bltpch.h"
#include <random>
#include <ctime>

namespace Bolt
{

	class BLT_API Random
	{
	private:
		static std::minstd_rand m_Gen;

		Random() = delete;

	public:
		static void Initialize();
		static void SetSeed(uint32_t seed);

		// Generates a random integer [low, high]
		static int NextInt(int low, int high);
		// Generates a random floating point [low, high)
		static float NextFloat(float low, float high);
		// Generates a random floating point [0, 1)
		static float NextFloat();

	};

}