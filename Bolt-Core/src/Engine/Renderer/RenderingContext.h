#pragma once
#include "RenderGroup.h"

namespace Bolt
{

	struct BLT_API RenderingContext
	{
	public:
		std::vector<RenderGroup> StaticMeshes;
	};

}