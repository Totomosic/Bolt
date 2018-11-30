#include "RenderSchedule.h"

namespace Bolt
{

	const std::vector<RenderPass>& RenderSchedule::RenderPasses() const
	{
		return m_RenderPasses;
	}

	RenderPass& RenderSchedule::GetRenderPass(id_t index)
	{
		return m_RenderPasses.at(index);
	}

	id_t RenderSchedule::AddPass(RenderPass pass)
	{
		if (pass.AdditionalLights.size() == 0)
		{
			pass.AdditionalLights.push_back(LightSource{ Vector3f(0, 1000, 0) });
			BLT_CORE_WARN("No Additional lights specified in render pass, Added default light at Position (0, 1000, 0)");
		}
		id_t index = m_RenderPasses.size();
		m_RenderPasses.push_back(std::move(pass));
		return index;
	}

}