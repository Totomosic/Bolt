#pragma once
#include "bltpch.h"

namespace Bolt
{

	struct BLT_API Frustum
	{
	public:
		float Left;
		float Right;
		float Bottom;
		float Top;
		float Near;
		float Far;

	public:
		float Width() const;
		float Height() const;
		float Depth() const;

	public:
		static Frustum Perspective(float fovy, float aspect, float nearPlane, float farPlane);

	};

}