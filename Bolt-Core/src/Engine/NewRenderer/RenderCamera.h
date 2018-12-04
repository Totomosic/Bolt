#pragma once
#include "Types.h"

namespace Bolt
{

	struct BLT_API RenderCamera
	{
	public:
		Matrix4f ViewMatrix;
		Matrix4f ProjectionMatrix;
	};

}