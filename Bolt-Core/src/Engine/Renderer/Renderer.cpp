#include "bltpch.h"
#include "Renderer.h"

#include "Graphics.h"

namespace Bolt
{

	Renderer::Renderer()
		: m_Scenes(), m_CurrentScene(nullptr)
	{
	}

	const RendererStats& Renderer::GetStats() const
	{
		return m_Stats;
	}

	void Renderer::BeginScene(const Framebuffer* renderTarget, const RenderCamera& camera, const RenderingContext& context)
	{
		BLT_ASSERT(m_CurrentScene == nullptr, "Current scene has not been ended");
		size_t index = m_Scenes.size();
		m_Scenes.push_back(RenderScene());
		RenderScene& scene = m_Scenes[index];
		scene.Camera = camera;
		scene.Context = context;
		scene.Pass.RenderTarget = renderTarget;
		m_CurrentScene = &scene;
	}

	void Renderer::Submit(const Mesh& mesh, const Matrix4f& transform)
	{
		BLT_ASSERT(m_CurrentScene != nullptr, "Current scene has not been started");
		for (const Mesh::ModelGroup& modelGroup : mesh.Models)
		{
			Matrix4f fullTransform = transform * modelGroup.Transform;
			const ModelData& data = modelGroup.Model->Data();
			for (int i = 0; i < data.Indices->IndexBufferCount(); i++)
			{
				const Material* material = mesh.Materials[modelGroup.MaterialIndices[i]].get();
				const ShaderInstance* shader = &material->GetLinkContext().GetShaderInstance();
				RenderData renderData;
				renderData.Transform = fullTransform;
				renderData.Vertices = data.Vertices.get();
				renderData.Indices = data.Indices->GetIndexBuffer(i).get();
				AddRenderData(shader, material, renderData);
			}
		}
	}

	void Renderer::EndScene()
	{
		BLT_ASSERT(m_CurrentScene != nullptr, "Current scene has not been started");
		m_CurrentScene = nullptr;
	}

	void Renderer::Flush()
	{
		BLT_ASSERT(m_CurrentScene== nullptr, "Current scene has not been ended");
		ResetStats();
		RenderRoutine routine;
		for (RenderScene& scene : m_Scenes)
		{
			m_Stats.SceneCount += 1;
			scene.Pass.RenderTarget->Bind();
			for (RenderGroup& group : scene.Pass.RenderGroups)
			{
				DrawRenderGroup(&routine, group, scene.Context, scene.Camera);
			}
		}
		m_Scenes.clear();
	}

	void Renderer::ResetStats()
	{
		m_Stats.SceneCount = 0;
		m_Stats.RenderGroupCount = 0;
		m_Stats.DrawCalls = 0;
	}

	void Renderer::DrawRenderGroup(RenderRoutine* routine, const RenderGroup& group, const RenderingContext& context, const RenderCamera& camera)
	{
		m_Stats.RenderGroupCount += 1;
		(*routine)(group, camera.ViewMatrix, camera.ProjectionMatrix, context, m_Stats);
	}

	void Renderer::AddRenderData(const ShaderInstance* shader, const Material* material, const RenderData& data)
	{
		SceneHelperData& helper = m_CurrentScene->Helper;
		if (helper.ShaderMap.find(shader) == helper.ShaderMap.end())
		{
			size_t shaderGroupIndex = m_CurrentScene->Pass.RenderGroups.size();
			size_t materialGroupIndex = 0;
			SceneHelperData::ShaderGroup shaderGroup;
			shaderGroup.Index = shaderGroupIndex;
			shaderGroup.MaterialGroups[material] = materialGroupIndex;
			helper.ShaderMap[shader] = shaderGroup;

			RenderGroup group;
			group.Shader = shader;
			MaterialPair pair;
			pair.Material = material;
			pair.Renderables.push_back(data);
			group.RenderData.push_back(pair);
			m_CurrentScene->Pass.RenderGroups.push_back(group);
		}
		else
		{
			SceneHelperData::ShaderGroup& shaderGroup = helper.ShaderMap[shader];
			if (shaderGroup.MaterialGroups.find(material) == shaderGroup.MaterialGroups.end())
			{
				RenderGroup& renderGroup = m_CurrentScene->Pass.RenderGroups[shaderGroup.Index];
				size_t materialGroupIndex = renderGroup.RenderData.size();
				shaderGroup.MaterialGroups[material] = materialGroupIndex;
				
				MaterialPair pair;
				pair.Material = material;
				pair.Renderables.push_back(data);
				renderGroup.RenderData.push_back(pair);
			}
			else
			{
				size_t materialGroupIndex = shaderGroup.MaterialGroups[material];
				m_CurrentScene->Pass.RenderGroups[shaderGroup.Index].RenderData[materialGroupIndex].Renderables.push_back(data);
			}
		}
	}

}