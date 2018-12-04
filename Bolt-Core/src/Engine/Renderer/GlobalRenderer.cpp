#include "GlobalRenderer.h"
#include "..\Scene\Lighting\LightSource.h"

namespace Bolt
{

	std::unique_ptr<RenderRoutine> GlobalRenderer::s_RenderRoutine = std::make_unique<RenderRoutine>();

	void GlobalRenderer::Render(const RenderPass& renderPass, const RenderContext& context, const RenderCamera& camera)
	{
		renderPass.Metadata.RenderTarget->Bind();
		if (renderPass.Metadata.ClearRenderTarget)
		{
			renderPass.Metadata.RenderTarget->Clear();
		}

		for (const RenderGroup& group : renderPass.RenderGroups)
		{
			context.Uniforms.UploadAll(group.Material->Shader.Get());
			group.Material->Shader->SetLights({ LightSource{ Vector3f(0, 100, 0) } });
			(*s_RenderRoutine)(group, camera.ViewMatrix, camera.ProjectionMatrix);
		}
	}

}