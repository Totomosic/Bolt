#include "SceneRenderer.h"

namespace Bolt
{

	void PopulateMaterialMap(std::vector<GameObject*>& objects, std::unordered_map<Material, std::vector<std::pair<const Material*, RenderData>>>& materialMap)
	{
		for (GameObject* object : objects)
		{
			if (!object->Components().HasComponent<MeshRenderer>())
			{
				continue;
			}
			Mesh& mesh = object->Components().GetComponent<MeshRenderer>().Mesh;
			for (Mesh::ModelGroup& model : mesh.Models)
			{
				const ModelData& modelData = model.Model->Data();
				for (int i = 0; i < modelData.Indices.IndexBufferCount(); i++)
				{
					const std::unique_ptr<IndexBuffer>& indexBuffer = modelData.Indices.GetIndexBuffer(i);
					const Material& material = mesh.Materials[model.MaterialIndices[i]];
					RenderData renderData;
					renderData.Indices = indexBuffer.get();
					renderData.Vertices = modelData.Vertices.get();
					renderData.Transform = object->transform().TransformMatrix() * model.Transform;
					materialMap[material].push_back({ &material, std::move(renderData) });
				}
			}
		}
	}

	void PopulateRenderPass(RenderPass& renderPass, std::unordered_map<Material, std::vector<std::pair<const Material*, RenderData>>>& materialMap)
	{
		for (auto& pair : materialMap)
		{
			RenderGroup group;
			group.Material = pair.second[0].first;
			for (const std::pair<const Material*, RenderData>& dataPair : pair.second)
			{
				group.Renderables.push_back(dataPair.second);
			}
			renderPass.RenderGroups.push_back(std::move(group));
		}
	}

	void SceneRenderer::Render(const RenderPassData& passData, const Scene& scene)
	{
		std::vector<const Layer*> layers = scene.GetAllLayers();
		passData.RenderTarget->Clear();
		for (const Layer* layer : layers)
		{
			RenderPass defaultRenderPass;
			RenderPass transparentPass;
			defaultRenderPass.Metadata = { passData.RenderTarget, false };
			transparentPass.Metadata = { passData.RenderTarget, false };
			ProjectionType projection = layer->ActiveCamera()->CameraProjection().Type;
			
			std::unordered_map<Material, std::vector<std::pair<const Material*, RenderData>>> materialMap;
			std::vector<GameObject*> objects = layer->Graph().Query(SGQTransparency(false)).GameObjects;

			PopulateMaterialMap(objects, materialMap);
			PopulateRenderPass(defaultRenderPass, materialMap);
			materialMap.clear();
			objects = layer->Graph().Query(SGQTransparency(true)).GameObjects;
			if (projection == ProjectionType::Orthographic)
			{
				std::sort(objects.begin(), objects.end(), [](GameObject* left, GameObject* right)
				{
					return left->transform().Position().z <= right->transform().Position().z;
				});
			}
			else if (projection == ProjectionType::Perspective)
			{
				Vector3f cameraPosition = layer->ActiveCamera()->transform().Position();
				std::sort(objects.begin(), objects.end(), [&cameraPosition](GameObject* left, GameObject* right)
				{
					return (cameraPosition - left->transform().Position()).LengthSqrd() >= (cameraPosition - right->transform().Position()).LengthSqrd();
				});
			}
			PopulateMaterialMap(objects, materialMap);
			PopulateRenderPass(transparentPass, materialMap);

			RenderCamera camera;
			camera.ViewMatrix = layer->ActiveCamera()->ViewMatrix();
			camera.ProjectionMatrix = layer->ActiveCamera()->ProjectionMatrix();
			GlobalRenderer::Render(defaultRenderPass, {}, camera);
			GlobalRenderer::Render(transparentPass, {}, camera);
		}
	}

}