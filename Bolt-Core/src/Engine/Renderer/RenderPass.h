#pragma once
#include "RenderData.h"
#include "Graphics/Framebuffer.h"

namespace Bolt
{

	struct BLT_API RenderPass
	{
	public:
		const Framebuffer* RenderTarget = nullptr;
		std::vector<RenderGroup> RenderGroups;
	};

}