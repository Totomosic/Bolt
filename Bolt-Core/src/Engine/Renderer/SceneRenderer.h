#pragma once
#include "GlobalRenderer.h"
#include "..\Scene\__Scene__.h"

namespace Bolt
{

	class BLT_API SceneRenderer
	{
	public:
		static void Render(const RenderPassData& passData, const Scene& scene);

	};

}