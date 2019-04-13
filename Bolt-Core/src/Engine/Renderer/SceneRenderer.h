#pragma once
#include "GlobalRenderer.h"
#include "RenderOptions.h"
#include "RenderSchedule.h"

namespace Bolt
{

	class BLT_API SceneRenderer
	{
	public:
		static constexpr id_t ALL_LAYERS = (id_t)-1;

	private:
		static std::unique_ptr<SceneRenderer> s_Instance;

	public:
		static SceneRenderer& Get();

	private:
		std::unordered_map<const Scene*, RenderSchedule> m_Schedules;

	public:
		SceneRenderer();

		void AddRenderSchedule(const RenderSchedule& schedule);

		// Execute render schedule
		void Render(const Scene& scene);
		// Render All layers of scene
		void Render(const RenderOptions& passData, const Scene& scene);
		// Render layers of scene specified by layerMask
		void Render(const RenderOptions& passData, const Scene& scene, id_t layerMask);
		// Render layer
		void RenderLayer(const RenderOptions& passData, const Layer& layer);

	};

}