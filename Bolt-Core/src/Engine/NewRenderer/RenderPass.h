#pragma once
#include "RenderGroup.h"

namespace Bolt
{

	struct BLT_API RenderPass
	{
	public:
		const Framebuffer* RenderTarget = nullptr;
		bool ClearRenderTarget = true;
		std::vector<RenderGroup> RenderGroups = std::vector<RenderGroup>();
	};

}