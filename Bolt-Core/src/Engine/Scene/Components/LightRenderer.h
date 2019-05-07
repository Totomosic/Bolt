#pragma once
#include "Component.h"
#include "../GameObject.h"
#include "../Lighting/LightSource.h"

namespace Bolt
{

	class BLT_API LightRenderer : public Component
	{
	public:
		LightType Type = LightType::Point;
		Color LightColor = Color::White;
		// ax^2 + bx + c
		Vector3f Attenuation = Vector3f(0.0f, 0.0f, 1.0f);
		float SpotAngle = (float)PI / 3.0f;

	public:
		LightRenderer(LightType type, const Color& lightColor, const Vector3f& attenuation = Vector3f(0.0f, 0.0f, 1.0f), float spotlightAngle = PI / 3.0f);
		LightRenderer();

		std::unique_ptr<Component> Clone() const override;

	};

}