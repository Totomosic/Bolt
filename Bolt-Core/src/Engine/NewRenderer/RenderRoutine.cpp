#include "RenderRoutine.h"

namespace Bolt
{

	void RenderRoutine::operator()(const RenderGroup& group, const Matrix4f& viewMatrix, const Matrix4f& projectionMatrix)
	{
		const Shader* shader = group.Material->Shader.Get();
		shader->SetMaterial(*group.Material);
		shader->SetViewMatrix(viewMatrix);
		shader->SetProjectionMatrix(projectionMatrix);

		for (const RenderData& data : group.Renderables)
		{
			uint renderCount = std::min((uint)data.Indices->IndexCount(), data.IndexCount);
			shader->SetModelMatrix(data.Transform);
			data.Vertices->Bind();
			data.Indices->Bind();
			GL_CALL(glDrawElements((GLenum)data.Vertices->GetRenderMode(), renderCount, data.Indices->IndexType(), nullptr));
		}
	}

}