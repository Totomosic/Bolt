#pragma once
#include "RenderGroup.h"
#include "Graphics/Framebuffer.h"

namespace Bolt
{

	struct BLT_API RenderPass
	{
	public:
		const Framebuffer* RenderTarget;
		std::vector<RenderGroup> RenderGroups = std::vector<RenderGroup>();
	};

}