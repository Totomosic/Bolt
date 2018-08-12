#pragma once
#include "Bolt-Core.h"

namespace Bolt
{

	struct BLT_API LightSettings
	{
	public:
		Color SpecularColor = Color::White;
		float Reflectivity = 0.0f;
		float ShineDamper = 10.0f;

		Color EmissionColor = Color::Black;
		float EmissionIntensity = 0.0f;

	public:
		bool operator==(const LightSettings& other) const;
		bool operator!=(const LightSettings& other) const;

	};

}