#pragma once
#include "GlobalRenderer.h"
#include "RenderOptions.h"
#include "RenderSchedule.h"

namespace Bolt
{

	class BLT_API SceneRenderer
	{
	public:
		inline static constexpr id_t ALL_LAYERS = (id_t)-1;

	private:
		static std::unordered_map<const Scene*, RenderSchedule> s_Schedules;

	public:
		static void AddRenderSchedule(const RenderSchedule& schedule);

		// Execute render schedule
		static void Render(const Scene& scene);
		// Render All layers of scene
		static void Render(const RenderOptions& passData, const Scene& scene);
		// Render layers of scene specified by layerMask
		static void Render(const RenderOptions& passData, const Scene& scene, id_t layerMask);
		// Render layer
		static void RenderLayer(const RenderOptions& passData, const Layer& layer);

	};

}