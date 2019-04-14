#include "Types.h"

#include "RenderRoutine.h"

namespace Bolt
{

	void RenderRoutine::operator()(const RenderGroup& group, const Matrix4f& viewMatrix, const Matrix4f& projectionMatrix)
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