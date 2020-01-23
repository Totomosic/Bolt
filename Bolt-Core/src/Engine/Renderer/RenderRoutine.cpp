#include "bltpch.h"

#include "RenderRoutine.h"
#include "Graphics/Resources/Meshes/Materials/Material.h"
#include "Core/Time/Time.h"
#include "Graphics.h"

namespace Bolt
{

	struct UniformLocations
	{
	public:
		const RendererUniformLocation* ModelMatrix = nullptr;
		const RendererUniformLocation* NormalMatrix = nullptr;
	};

	// Applies Renderer Uniforms and returns the location of RendererUniform::ModelMatrix and RendererUniform::NormalMatrix
	UniformLocations ApplyRendererUniforms(const ShaderInstance& shaderInstance, const Matrix4f& viewMatrix, const Matrix4f& projectionMatrix, const RenderingContext& context)
	{
		const Shader& shader = shaderInstance.GetShader();
		UniformLocations locations;
		for (const RendererUniformLocation& uniform : shaderInstance.GetRendererUniforms())
		{
			switch (uniform.Uniform)
			{
			case RendererUniform::ModelMatrix:
				locations.ModelMatrix = &uniform;
				break;
			case RendererUniform::ViewMatrix:
				shader.SetUniform(uniform.Location, viewMatrix);
				break;
			case RendererUniform::ProjectionMatrix:
				shader.SetUniform(uniform.Location, projectionMatrix);
				break;
			case RendererUniform::NormalMatrix:
				locations.NormalMatrix = &uniform;
				break;
			case RendererUniform::Time:
				shader.SetUniform(uniform.Location, Time::Get().RenderingTimeline().CurrentTime());
				break;
			case RendererUniform::CameraPosition:
			{
				Vector3f camPos = (Vector3f)viewMatrix.Inverse().Column(3).xyz();
				shader.SetUniform(uniform.Location, camPos);
				break;
			}
			case RendererUniform::CameraDirection:
				shader.SetUniform(uniform.Location, (Vector3f)viewMatrix.Inverse().Row(2).xyz() * Vector3f(1, 1, -1));
				break;
			case RendererUniform::LightPositions:
				if (uniform.Index < context.LightSources.size())
				{
					shader.SetUniform(uniform.Location, context.LightSources.at(uniform.Index).Position);
				}
				break;
			case RendererUniform::LightColors:
				if (uniform.Index < context.LightSources.size())
				{
					shader.SetUniform(uniform.Location, context.LightSources.at(uniform.Index).LightData.Color);
				}
				break;
			case RendererUniform::LightAmbients:
				if (uniform.Index < context.LightSources.size())
				{
					shader.SetUniform(uniform.Location, context.LightSources.at(uniform.Index).LightData.Ambient);
				}
				break;
			case RendererUniform::LightAmbientColors:
				if (uniform.Index < context.LightSources.size())
				{
					shader.SetUniform(uniform.Location, context.LightSources.at(uniform.Index).LightData.AmbientColor);
				}
				break;
			case RendererUniform::LightIntensities:
				if (uniform.Index < context.LightSources.size())
				{
					shader.SetUniform(uniform.Location, context.LightSources.at(uniform.Index).LightData.Intensity);
				}
				break;
			case RendererUniform::LightAttenuations:
				if (uniform.Index < context.LightSources.size())
				{
					shader.SetUniform(uniform.Location, context.LightSources.at(uniform.Index).LightData.Attenuation);
				}
				break;
			case RendererUniform::LightCount:
				shader.SetUniform(uniform.Location, (int)context.LightSources.size());
				break;
			default:
				break;
			}
		}
		return locations;
	}

	void RenderRoutine::operator()(const RenderGroup& group, const Matrix4f& viewMatrix, const Matrix4f& projectionMatrix, const RenderingContext& context, RendererStats& stats)
	{
		const ShaderInstance& shader = *group.Shader;
		shader.GetShader().Bind();
		UniformLocations locations = ApplyRendererUniforms(shader, viewMatrix, projectionMatrix, context);
		for (const MaterialPair& pair : group.RenderData)
		{
			const Material& material = *pair.Material;
			Graphics::Get().GetState().ApplySettings(material.GetRenderSettings());
			material.GetLinkContext().ApplyLinks();			
			for (const RenderData& data : pair.Renderables)
			{
				uint32_t renderCount = std::min((uint32_t)data.Indices->IndexCount(), data.IndexCount);
				if (locations.ModelMatrix != nullptr)
				{
					shader.GetShader().SetUniform(locations.ModelMatrix->Location, data.Transform);
				}
				if (locations.NormalMatrix != nullptr)
				{
					shader.GetShader().SetUniform(locations.NormalMatrix->Location, data.Transform.Inverse().Transpose());
				}
				data.Vertices->Bind();
				data.Indices->Bind();
				stats.DrawCalls += 1;
				GL_CALL(glDrawElements((GLenum)data.Vertices->GetRenderMode(), renderCount, data.Indices->IndexType(), nullptr));
			}
		}
	}

}