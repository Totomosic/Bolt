#pragma once
#include "RenderGroup.h"
#include "Engine/Scene/Lighting/LightSource.h"

namespace Bolt
{

	struct BLT_API RenderingContext
	{
	public:
		std::vector<RenderGroup> StaticMeshes;
		std::vector<LightSource> Lights;
	};

}