#include "Types.h"

#include "LightRenderer.h"

namespace Bolt
{

	LightRenderer::LightRenderer(LightType type, const Color& lightColor, const Vector3f& attenuation, float splotlightAngle) : Component(),
		Type(type), LightColor(lightColor), Attenuation(attenuation), SpotAngle(splotlightAngle)
	{
		
	}

	LightRenderer::LightRenderer() : LightRenderer(LightType::Point, Color::White)
	{
	
	}

	std::unique_ptr<Component> LightRenderer::Clone() const
	{
		return std::make_unique<LightRenderer>(Type, LightColor, Attenuation, SpotAngle);
	}

}