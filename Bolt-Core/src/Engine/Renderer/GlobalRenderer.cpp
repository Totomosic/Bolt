#include "GlobalRenderer.h"
#include "..\Scene\Lighting\LightSource.h"

namespace Bolt
{

	std::unique_ptr<RenderRoutine> GlobalRenderer::s_RenderRoutine = std::make_unique<RenderRoutine>();

	void DrawRenderGroup(RenderRoutine* routine, const RenderGroup& group, const RenderContext& context, const RenderCamera& camera)
	{
		context.Uniforms.UploadAll(group.Material->Shader.Get());
		group.Material->Shader->SetLights(context.Lights);
		(*routine)(group, camera.ViewMatrix, camera.ProjectionMatrix);
	}

	void GlobalRenderer::Render(const RenderPass& renderPass, const RenderContext& context, const RenderCamera& camera)
	{
		renderPass.RenderTarget->Bind();
		for (const RenderGroup& group : context.StaticMeshes)
		{
			DrawRenderGroup(s_RenderRoutine.get(), group, context, camera);
		}
		for (const RenderGroup& group : renderPass.RenderGroups)
		{
			DrawRenderGroup(s_RenderRoutine.get(), group, context, camera);
		}
	}

}