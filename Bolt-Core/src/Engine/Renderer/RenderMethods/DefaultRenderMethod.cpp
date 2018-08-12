#include "DefaultRenderMethod.h"

namespace Bolt
{

	Matrix3f CalculateMatrix3(const TextureBounds& bounds)
	{
		float width = bounds.MaxX - bounds.MinX;
		float height = bounds.MaxY - bounds.MinY;
		Matrix3f result = Matrix3f::Translation(bounds.MinX, bounds.MinY, 0) * Matrix3f::Scale(width, height, 1);
		return result;
	}

	void ApplyMaterial(const Material& material)
	{
		material.Shader->Bind();
		material.Shader->SetUniform("u_Color", material.BaseColor);
		material.Uniforms.UploadAll(material.Shader);
		GLState::ApplySettings(material.RenderOptions);

		//const std::vector<Shader::UniformVariable>& textureSamplers = material.Shader->GetAllTextureSamplers();
		for (int i = 0; i < material.Textures.Textures.size(); i++)
		{
			material.Textures.Textures[i]->Bind(i);
			material.Shader->SetUniform("u_Textures[" + std::to_string(i) + ']', i);
			Matrix3f textureMatrix = Matrix3f::Identity();
			if (material.Textures.Animators.find(i) != material.Textures.Animators.end())
			{
				textureMatrix = CalculateMatrix3(material.Textures.Animators.at(i)->GetBounds());
			}
			material.Shader->SetUniform("u_TexTransforms[" + std::to_string(i) + ']', textureMatrix);
		}
	}

	void DefaultRenderMethod::operator()(const RenderBatch& batch, const Matrix4f& viewMatrix, const Matrix4f& projectionMatrix) const
	{
		const Shader* shader = batch.Material->Shader;
		ApplyMaterial(*batch.Material);
		shader->SetUniform("u_ViewMatrix", viewMatrix);
		shader->SetUniform("u_ProjectionMatrix", projectionMatrix);

		for (const GeometryData& data : batch.Geometry)
		{
			int renderCount = min(data.Indices->IndexCount(), data.IndexOverride);
			shader->SetUniform("u_ModelMatrix", data.Transform);
			data.Vertices->Bind();
			data.Indices->Bind();
			glDrawElements((GLenum)data.Vertices->GetRenderMode(), renderCount, data.Indices->IndexType(), nullptr);
		}
	}

}