#include "Renderer.h"
#include "..\Scene\SceneGraph\Query\__Query__.h"

namespace Bolt
{

	Renderer::Renderer(std::unique_ptr<RenderMethod>&& renderMethod)
		: m_Method(std::move(renderMethod))
	{
	
	}

	void Renderer::Begin(const RenderPass* renderPass) const
	{
		renderPass->RenderTarget->Bind();
	}

	void Renderer::Render(const RenderPass* renderPass) const
	{
		if (&SceneManager::CurrentScene() == nullptr)
		{
			return;
		}
		std::vector<const Layer*> layers = (renderPass->LayerMask == RenderPass::ALL_LAYERS) ? SceneManager::CurrentScene().GetAllLayers() : SceneManager::CurrentScene().GetLayers(renderPass->LayerMask);
		for (const Layer* layer : layers)
		{
			std::vector<GameObject*> defaultObjects = layer->Graph().Query(SGQTransparency(false)).GameObjects;
			std::vector<GameObject*> transparentObjects = layer->Graph().Query(SGQTransparency(true)).GameObjects;

			Camera* camera = layer->ActiveCamera();
			if (camera->CameraProjection().Type == ProjectionType::Orthographic)
			{
				std::sort(transparentObjects.begin(), transparentObjects.end(), [camera](const GameObject* first, const GameObject* second) {

					float d2 = -(second->transform().Position().z - camera->transform().Position().z);
					float d1 = -(first->transform().Position().z - camera->transform().Position().z);
					return d1 < d2;
				});
			}
			else
			{
				std::sort(transparentObjects.begin(), transparentObjects.end(), [camera](const GameObject* first, const GameObject* second) {

					float d2 = (second->transform().Position() - camera->transform().Position()).Length();
					float d1 = (first->transform().Position() - camera->transform().Position()).Length();
					return d1 < d2;
				});
			}

			bool hasCameraOverride = renderPass->CameraOverrides.find(layer->Id()) != renderPass->CameraOverrides.end();
			Matrix4f viewMatrix = (hasCameraOverride) ? renderPass->CameraOverrides.at(layer->Id()).ViewMatrix : layer->ActiveCamera()->ViewMatrix();
			Matrix4f projectionMatrix = (hasCameraOverride) ? renderPass->CameraOverrides.at(layer->Id()).ProjectionMatrix : layer->ActiveCamera()->ProjectionMatrix();

			RenderGameObjects(defaultObjects, viewMatrix, projectionMatrix, renderPass->Uniforms);
			RenderGameObjects(transparentObjects, viewMatrix, projectionMatrix, renderPass->Uniforms);
		}
	}

	void Renderer::End(const RenderPass* renderPass) const
	{
		
	}

	void Renderer::RenderGameObjects(const std::vector<GameObject*>& objects, const Matrix4f& viewMatrix, const Matrix4f& projectionMatrix, const UniformManager& uniforms) const
	{
		std::unordered_map<Material, RenderBatch> renderBatches;		
		for (GameObject* object : objects)
		{
			const Mesh& mesh = object->Components().GetComponent<MeshRenderer>().Mesh;
			for (int i = 0; i < mesh.Models.size(); i++)
			{
				const Mesh::ModelGroup& model = mesh.Models[i];
				BLT_ASSERT(model.MaterialIndices.size() >= model.Model->Data().Indices.IndexBufferCount(), "Size of MaterialIndices should be equal to or greater than the number of Index Buffers Attached");
				for (int j = 0; j < model.MaterialIndices.size(); j++)
				{
					const Material* material = &mesh.Materials[model.MaterialIndices[j]];
					RenderBatch& batch = renderBatches[*material];
					batch.Material = material;
					batch.Geometry.push_back({ model.Model->Data().Vertices.get(), model.Model->Data().Indices.GetIndexBuffer(j).get(), object->transform().TransformMatrix() * model.Transform });
				}
			}
		}
		for (const auto& pair : renderBatches)
		{
			uniforms.UploadAll(pair.second.Material->Shader.Get());
			(*m_Method)(pair.second, viewMatrix, projectionMatrix);
		}
	}

}