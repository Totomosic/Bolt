#pragma once
#include "RenderGroup.h"

namespace Bolt
{

	struct BLT_API RenderContext
	{
	public:
		std::vector<RenderGroup> StaticMeshes;
		UniformManager Uniforms;
	};

}