#pragma once
#include "../Scene/Components/LightSource.h"

namespace Bolt
{

	struct BLT_API RenderCamera
	{
	public:
		Matrix4f ViewMatrix;
		Matrix4f ProjectionMatrix;
	};

	struct BLT_API RenderingContext
	{
	public:
		std::vector<LightSourceInstance> LightSources;
	};

}