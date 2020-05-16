#pragma once
#include "RenderingContext.h"
#include "Graphics/Assets/Meshes/Model.h"
#include "Graphics/Framebuffer.h"
#include "RenderPass.h"
#include "RenderRoutine.h"

#include "RendererStats.h"

namespace Bolt
{

	struct BLT_API SceneHelperData
	{
	public:
		struct BLT_API ShaderGroup
		{
		public:
			size_t Index;
			std::unordered_map<const Material*, size_t> MaterialGroups;
		};
	public:
		std::unordered_map<const ShaderInstance*, ShaderGroup> ShaderMap;
	};

	struct BLT_API RenderScene
	{
	public:
		RenderCamera Camera;
		RenderingContext Context;
		RenderPass Pass;

		SceneHelperData Helper;
	};

	class BLT_API Renderer
	{
	private:
		std::vector<RenderScene> m_Scenes;
		RenderScene* m_CurrentScene;
		RendererStats m_Stats;

	public:
		Renderer();

		const RendererStats& GetStats() const;

		void BeginScene(const Framebuffer* renderTarget, const RenderCamera& camera, const RenderingContext& context);
		void Submit(const Model& model, const Matrix4f& transform);
		void EndScene();

		void Flush();

	private:
		void ResetStats();
		void DrawRenderGroup(RenderRoutine* routine, const RenderGroup& group, const RenderingContext& context, const RenderCamera& camera);
		void AddRenderData(const ShaderInstance* shader, const Material* material, const RenderData& data);
	};

}