#pragma once
#include "RenderGroup.h"

namespace Bolt
{

	struct BLT_API RenderPassData
	{
	public:
		const Framebuffer* RenderTarget = nullptr;
		bool ClearRenderTarget = true;
	};

	struct BLT_API RenderPass
	{
	public:
		RenderPassData Metadata = RenderPassData();
		std::vector<RenderGroup> RenderGroups = std::vector<RenderGroup>();
	};

}