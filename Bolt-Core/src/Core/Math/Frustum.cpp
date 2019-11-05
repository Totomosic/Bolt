#include "bltpch.h"

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
		float height = nearPlane * tan(fovy / 2.0f);
		float width = height * aspect;
		Frustum result = { -width, width, -height, height, nearPlane, farPlane };
		return result;
	}

}