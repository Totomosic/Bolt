#pragma once
#include "Bolt-Graphics.h"

namespace Bolt
{

	BLT_API enum class LightType
	{
		Point,
		Directional,
		Spotlight
	};

	struct BLT_API LightSource
	{
	public:
		Vector3f Position = Vector3f(0.0f, 0.0f, 0.0f);
		Color Color = Color::White;
		float AmbientIntensity = 0.2f;
		Vector3f Attenuation = Vector3f(1.0f, 0.0f, 0.0f);

		Vector3f ForwardDirection = Vector3f::Forward();
		float SpotlightAngle = (float)PI / 3.0f;

	};

}