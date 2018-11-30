#pragma once
#include "RenderPass.h"

namespace Bolt
{

	struct BLT_API RenderSchedule
	{
	private:
		std::vector<RenderPass> m_RenderPasses;

	public:
		RenderSchedule() = default;

		const std::vector<RenderPass>& RenderPasses() const;
		RenderPass& GetRenderPass(id_t index);

		id_t AddPass(RenderPass pass);

	};

}