#include "Types.h"

#include "RenderRoutine.h"
#include "Graphics/Resources/Meshes/Materials/Material.h"
#include "Core/Time/Time.h"

namespace Bolt
{

	void RenderRoutine::operator()(const RenderGroup& group, const Matrix4f& viewMatrix, const Matrix4f& projectionMatrix, const RenderingContext& context)
	{
		const Material* material = group.Material;
		const Shader& shader = material->GetShader().GetShaderInstance().GetShader();
		for (const RendererUniformLocation& uniform : material->GetShader().GetShaderInstance().GetRendererUniforms())
		{
			switch (uniform.Uniform)
			{
			case RendererUniform::ViewMatrix:
				shader.SetUniform(uniform.Location, viewMatrix);
				break;
			case RendererUniform::ProjectionMatrix:
				shader.SetUniform(uniform.Location, projectionMatrix);
				break;
			case RendererUniform::Time:
				shader.SetUniform(uniform.Location, Time::Get().RenderingTimeline().CurrentTime());
				break;
			case RendererUniform::LightPositions:
				if (uniform.Index < context.Lights.size())
				{
					shader.SetUniform(uniform.Location, context.Lights.at(uniform.Index).Position);
				}
				break;
			case RendererUniform::LightColors:
				if (uniform.Index < context.Lights.size())
				{
					shader.SetUniform(uniform.Location, context.Lights.at(uniform.Index).Color);
				}
				break;
			case RendererUniform::LightAmbients:
				if (uniform.Index < context.Lights.size())
				{
					shader.SetUniform(uniform.Location, context.Lights.at(uniform.Index).AmbientIntensity);
				}
				break;
			case RendererUniform::LightCount:
				shader.SetUniform(uniform.Location, context.Lights.size());
				break;
			}
		}
		for (const RenderData& data : group.Renderables)
		{
			uint renderCount = std::min((uint)data.Indices->IndexCount(), data.IndexCount);
			for (const RendererUniformLocation& uniform : material->GetShader().GetShaderInstance().GetRendererUniforms())
			{
				if (uniform.Uniform == RendererUniform::ModelMatrix)
				{
					shader.SetUniform(uniform.Location, data.Transform);
				}
			}
			data.Vertices->Bind();
			data.Indices->Bind();
			GL_CALL(glDrawElements((GLenum)data.Vertices->GetRenderMode(), renderCount, data.Indices->IndexType(), nullptr));
		}
	}

}