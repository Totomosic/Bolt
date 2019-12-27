#pragma once
#include "Core/Math/Vectors/Vector3.h"
#include "Core/Color.h"

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
		Vector3f Position = {};
		LightType Type = LightType::Point;
		Bolt::Color Color = Bolt::Color::White;
		float Intensity = 1.0f;
		float Ambient = 0.3f;
		Vector3f Direction = Vector3f::Forward();
		Vector3f Attenuation = { 1.0f, 0.0f, 0.0f };
		float SpotAngle = 0.0f;
	};

}