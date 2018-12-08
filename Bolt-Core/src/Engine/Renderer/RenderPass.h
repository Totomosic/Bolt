#pragma once
#include "RenderGroup.h"

namespace Bolt
{

	struct BLT_API RenderPass
	{
	public:
		const Framebuffer* RenderTarget;
		std::vector<RenderGroup> RenderGroups = std::vector<RenderGroup>();
	};

}