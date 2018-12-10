#include "Types.h"
#include "Frustum.h"

namespace Bolt
{

	float Frustum::Width() const
	{
		return Right - Left;
	}

	float Frustum::Height() const
	{
		return Top - Bottom;
	}

	float Frustum::Depth() const
	{
		return Far - Near;
	}

	Frustum Frustum::Perspective(float fovy, float aspect, float nearPlane, float farPlane)
	{
		float height = nearPlane * tan(fovy);
		float width = height * aspect;
		Frustum result = { -width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, nearPlane, farPlane };
		return result;
	}

}