#pragma once
#include "Bolt-Graphics.h"
#include "..\Scene\Camera.h"
#include "..\Scene\Lighting\LightSource.h"

namespace Bolt
{

	class Renderer;

	struct BLT_API RenderPass
	{
	public:
		static constexpr id_t ALL_LAYERS = (id_t)-1;

	public:
		const Framebuffer* RenderTarget;
		id_t LayerMask;
		const Renderer* PassRenderer;
		std::unordered_map<id_t, CameraView> CameraOverrides;
		std::vector<Plane> ClippingPlanes;
		UniformManager Uniforms;
		std::vector<LightSource> AdditionalLights;

	};

}