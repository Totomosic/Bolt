#include "Types.h"

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

	std::unordered_map<const Scene*, RenderSchedule> SceneRenderer::s_Schedules = std::unordered_map<const Scene*, RenderSchedule>();

	void SceneRenderer::AddRenderSchedule(const RenderSchedule& schedule)
	{
		BLT_ASSERT(s_Schedules.find(&schedule.GetScene()) == s_Schedules.end(), "Schedule already exists for this scene");
		s_Schedules[&schedule.GetScene()] = schedule;
	}

	void SceneRenderer::Render(const Scene& scene)
	{
		BLT_ASSERT(s_Schedules.find(&scene) != s_Schedules.end(), "No schedule found for given scene");
		RenderSchedule& schedule = s_Schedules[&scene];
		for (const RenderProcess& process : schedule.Processes())
		{
			std::vector<const Layer*> layers = (process.LayerMask == SceneRenderer::ALL_LAYERS) ? scene.GetAllLayers() : scene.GetLayers(process.LayerMask);
			for (const Layer* layer : layers)
			{
				RenderOptions options = process.Options;
				if (process.Cameras.find(layer->Id()) != process.Cameras.end())
				{
					options.CameraOverride = process.Cameras.at(layer->Id());
				}
				// Always clear the depth buffer between layers
				process.Options.RenderTarget->Clear(ClearBuffer::Depth);
				RenderLayer(options, *layer);
			}
		}
	}

	void SceneRenderer::Render(const RenderOptions& options, const Scene& scene)
	{
		return Render(options, scene, SceneRenderer::ALL_LAYERS);
	}

	void SceneRenderer::Render(const RenderOptions& options, const Scene& scene, id_t layerMask)
	{
		std::vector<const Layer*> layers = (layerMask != SceneRenderer::ALL_LAYERS) ? scene.GetLayers(layerMask) : scene.GetAllLayers();
		for (const Layer* layer : layers)
		{
			RenderLayer(options, *layer);
		}
	}

	void SceneRenderer::RenderLayer(const RenderOptions& passData, const Layer& layer)
	{
		RenderPass defaultRenderPass;
		RenderPass transparentPass;
		defaultRenderPass.RenderTarget = passData.RenderTarget;
		transparentPass.RenderTarget = passData.RenderTarget;
		ProjectionType projection = layer.ActiveCamera()->CameraProjection().Type;

		std::unordered_map<Material, std::vector<std::pair<const Material*, RenderData>>> materialMap;
		std::vector<GameObject*> objects = layer.GameObjects().Query(SGQTransparency(false)).GameObjects;

		PopulateMaterialMap(objects, materialMap);
		PopulateRenderPass(defaultRenderPass, materialMap);
		materialMap.clear();
		objects = layer.GameObjects().Query(SGQTransparency(true)).GameObjects;
		if (projection == ProjectionType::Orthographic)
		{
			std::sort(objects.begin(), objects.end(), [](GameObject* left, GameObject* right)
			{
				return left->transform().Position().z >= right->transform().Position().z;
			});
		}
		else if (projection == ProjectionType::Perspective)
		{
			Vector3f cameraPosition = layer.ActiveCamera()->transform().Position();
			std::sort(objects.begin(), objects.end(), [&cameraPosition](GameObject* left, GameObject* right)
			{
				return (cameraPosition - left->transform().Position()).LengthSqrd() >= (cameraPosition - right->transform().Position()).LengthSqrd();
			});
		}
		PopulateMaterialMap(objects, materialMap);
		PopulateRenderPass(transparentPass, materialMap);

		RenderContext context = passData.GlobalContext;
		if (context.Lights.size() == 0)
		{
			context.Lights.push_back(LightSource{ Vector3f(0, 100, 0) });
		}

		RenderCamera camera;
		camera.ViewMatrix = (passData.CameraOverride.ViewMatrix == nullptr) ? layer.ActiveCamera()->ViewMatrix() : *passData.CameraOverride.ViewMatrix;
		camera.ProjectionMatrix = (passData.CameraOverride.ProjectionMatrix == nullptr) ? layer.ActiveCamera()->ProjectionMatrix() : *passData.CameraOverride.ProjectionMatrix;
		GlobalRenderer::Render(defaultRenderPass, context, camera);
		GlobalRenderer::Render(transparentPass, context, camera);
	}

}