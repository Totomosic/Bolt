#pragma once
#include "..\RandomDistribution.h"

namespace Bolt
{

	struct BLT_API SimplexNoise
	{
	private:
		float m_Frequency;
		float m_Amplitude;
		float m_Lacunarity;
		float m_Persistence;

	public:
		// Frequency = "width" of first octave, Amplitude = "Height" of first octave, Lacunarity = frequency multiplier between octaves, Persistance = loss of amplitude between octaves (usually 1/lucanarity)
		SimplexNoise(float frequency = 1.0f, float amplitude = 1.0f, float lacunarity = 2.0f, float persistence = 0.5f);

		float Noise(float x) const;
		float Noise(float x, float y) const;
		float Noise(float x, float y, float z) const;

		float Generate(size_t octaves, float x) const;
		float Generate(size_t octaves, float x, float y) const;
		float Generate(size_t octaves, float x, float y, float z) const;

	};

}