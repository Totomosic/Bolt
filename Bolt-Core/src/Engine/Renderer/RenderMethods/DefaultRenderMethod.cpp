#include "DefaultRenderMethod.h"

namespace Bolt
{

	void DefaultRenderMethod::operator()(const RenderBatch& batch, const Matrix4f& viewMatrix, const Matrix4f& projectionMatrix) const
	{
		const Shader* shader = batch.Material->Shader.Get();
		batch.Material->Shader->SetMaterial(*batch.Material);
		shader->SetViewMatrix(viewMatrix);
		shader->SetProjectionMatrix(projectionMatrix);

		for (const GeometryData& data : batch.Geometry)
		{
			int renderCount = std::min((uint)data.Indices->IndexCount(), data.IndexOverride);
			shader->SetModelMatrix(data.Transform);
			data.Vertices->Bind();
			data.Indices->Bind();
			glDrawElements((GLenum)data.Vertices->GetRenderMode(), renderCount, data.Indices->IndexType(), nullptr);
		}
	}

}