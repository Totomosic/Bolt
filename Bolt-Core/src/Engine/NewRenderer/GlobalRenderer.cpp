#include "GlobalRenderer.h"

namespace Bolt
{

	std::unique_ptr<RenderRoutine> GlobalRenderer::s_RenderRoutine = std::make_unique<RenderRoutine>();

	void GlobalRenderer::Render(const RenderPass& renderPass, const RenderContext& context, const RenderCamera& camera)
	{
		renderPass.RenderTarget->Bind();
		if (renderPass.ClearRenderTarget)
		{
			renderPass.RenderTarget->Clear();
		}

		for (const RenderGroup& group : renderPass.RenderGroups)
		{
			context.Uniforms.UploadAll(group.Material->Shader.Get());
			(*s_RenderRoutine)(group, camera.ViewMatrix, camera.ProjectionMatrix);
		}
	}

}