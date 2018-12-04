#pragma once
#include "RenderData.h"

namespace Bolt
{

	struct BLT_API RenderGroup
	{
	public:
		const Material* Material;
		std::vector<RenderData> Renderables;
	};

}