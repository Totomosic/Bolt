#include "Types.h"
#include "GlobalRenderer.h"
#include "Graphics.h"
#include "Engine/Scene/Lighting/LightSource.h"
#include "Graphics/Framebuffer.h"

namespace Bolt
{

	std::unique_ptr<RenderRoutine> GlobalRenderer::s_RenderRoutine = std::make_unique<RenderRoutine>();

	void DrawRenderGroup(RenderRoutine* routine, const RenderGroup& group, const RenderingContext& context, const RenderCamera& camera)
	{
		Graphics::Get().GetState().ApplySettings(group.Material->GetRenderSettings());
		group.Material->GetShader().ApplyLinks();
		(*routine)(group, camera.ViewMatrix, camera.ProjectionMatrix);
	}

	void GlobalRenderer::Render(const RenderPass& renderPass, const RenderingContext& context, const RenderCamera& camera)
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