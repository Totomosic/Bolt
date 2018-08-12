#pragma once
#include "RenderPass.h"

namespace Bolt
{

	struct BLT_API RenderSchedule
	{
	public:
		std::vector<RenderPass> RenderPasses;

	};

}