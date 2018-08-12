#include "LightSettings.h"

namespace Bolt
{

	bool LightSettings::operator==(const LightSettings& other) const
	{
		return (SpecularColor == other.SpecularColor && Reflectivity == other.Reflectivity && ShineDamper == other.ShineDamper && EmissionColor == other.EmissionColor && EmissionIntensity == other.EmissionIntensity);
	}

	bool LightSettings::operator!=(const LightSettings& other) const
	{
		return !(*this == other);
	}

}