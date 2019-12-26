#pragma once
#include "RenderData.h"
#include "Graphics/Resources/Meshes/Materials/Material.h"

namespace Bolt
{

	struct BLT_API RenderGroup
	{
	public:
		const Material* Material = nullptr;
		std::vector<RenderData> Renderables;
	};

}