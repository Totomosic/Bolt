#include "bltpch.h"

#include "SceneRenderer.h"
#include "Engine/Engine.h"
#include "Engine/Scene/Query/SGQTransparency.h"
#include "Engine/Scene/GameObject.h"
#include "Graphics/Resources/Meshes/Mesh.h"
#include "Engine/Scene/Components/MeshRenderer.h"

namespace Bolt
{

	void PopulateMaterialMap(std::vector<GameObject*>& objects, std::vector<std::pair<const Material*, std::vector<RenderData>>>& materialMap)
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
				if (model.Model != nullptr)
				{
					const ModelData& modelData = model.Model->Data();
					if (modelData.Vertices != nullptr && modelData.Indices != nullptr && !modelData.Vertices->IsMapped() && !modelData.Indices->IsMapped())
					{
						for (int i = 0; i < modelData.Indices->IndexBufferCount(); i++)
						{
							const std::unique_ptr<IndexBuffer>& indexBuffer = modelData.Indices->GetIndexBuffer(i);
							const std::unique_ptr<Material>& material = mesh.Materials[model.MaterialIndices[i]];
							RenderData renderData;
							renderData.Indices = indexBuffer.get();
							renderData.Vertices = modelData.Vertices.get();
							renderData.Transform = object->transform().TransformMatrix() * model.Transform;
							bool found = false;
							for (auto& pair : materialMap)
							{
								if (pair.first == material.get())
								{
									pair.second.push_back(std::move(renderData));
									found = true;
								}
							}
							if (!found)
							{
								std::vector<RenderData> data = { std::move(renderData) };
								materialMap.push_back({ material.get(), std::move(data) });
							}
						}
					}
				}
			}
		}
	}

	void PopulateRenderPass(RenderPass& renderPass, std::vector<std::pair<const Material*, std::vector<RenderData>>>& materialMap)
	{
		for (auto& pair : materialMap)
		{
			RenderGroup group;
			group.Material = pair.first;
			for (const RenderData& data : pair.second)
			{
				group.Renderables.push_back(data);
			}
			renderPass.RenderGroups.push_back(std::move(group));
		}
	}

	SceneRenderer& SceneRenderer::Get()
	{
		return Engine::Instance().CurrentContext().GetRenderContext().GetSceneRenderer();
	}

	SceneRenderer::SceneRenderer()
		: m_Schedules()
	{
	
	}

	void SceneRenderer::AddRenderSchedule(const RenderSchedule& schedule)
	{
		BLT_ASSERT(m_Schedules.find(&schedule.GetScene()) == m_Schedules.end(), "Schedule already exists for this scene");
		m_Schedules[&schedule.GetScene()] = schedule;
	}

	void SceneRenderer::Render(const Scene& scene)
	{
		BLT_ASSERT(m_Schedules.find(&scene) != m_Schedules.end(), "No schedule found for given scene");
		RenderSchedule& schedule = m_Schedules[&scene];
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
				if (options.RenderTarget == nullptr)
				{
					options.RenderTarget = Graphics::Get().DefaultFramebuffer();
				}
				options.RenderTarget->Clear(ClearBuffer::Depth);
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

		std::vector<std::pair<const Material*, std::vector<RenderData>>> materialMap;
		std::vector<GameObject*> objects = layer.GameObjects().Query(SGQTransparency(false)).GameObjects;

		PopulateMaterialMap(objects, materialMap);
		PopulateRenderPass(defaultRenderPass, materialMap);
		materialMap.clear();
		objects = layer.GameObjects().Query(SGQTransparency(true)).GameObjects;
		if (projection == ProjectionType::Orthographic)
		{
			std::sort(objects.begin(), objects.end(), [](GameObject* left, GameObject* right)
			{
				return left->transform().Position().z < right->transform().Position().z;
			});
		}
		else if (projection == ProjectionType::Perspective)
		{
			Vector3f cameraPosition = layer.ActiveCamera()->transform().Position();
			std::sort(objects.begin(), objects.end(), [&cameraPosition](GameObject* left, GameObject* right)
			{
				return (cameraPosition - right->transform().Position()).LengthSqrd() < (cameraPosition - left->transform().Position()).LengthSqrd();
			});
		}
		PopulateMaterialMap(objects, materialMap);
		PopulateRenderPass(transparentPass, materialMap);

		RenderingContext context = passData.GlobalContext;
		if (context.Lights.empty())
		{
			LightSource globalLight;
			globalLight.Position = { 0, 100, 0 };
			globalLight.Color = Color::White;
			globalLight.AmbientIntensity = 0.05f;
			globalLight.Attenuation = { 1, 0, 0 };
			globalLight.Intensity = 10;
			context.Lights.push_back(globalLight);
		}
		RenderCamera camera;
		camera.ViewMatrix = (passData.CameraOverride.ViewMatrix == nullptr) ? layer.ActiveCamera()->ViewMatrix() : *passData.CameraOverride.ViewMatrix;
		camera.ProjectionMatrix = (passData.CameraOverride.ProjectionMatrix == nullptr) ? layer.ActiveCamera()->ProjectionMatrix() : *passData.CameraOverride.ProjectionMatrix;
		GlobalRenderer::Render(defaultRenderPass, context, camera);
		GlobalRenderer::Render(transparentPass, context, camera);
	}

}