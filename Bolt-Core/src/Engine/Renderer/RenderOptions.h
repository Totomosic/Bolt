#pragma once
#include "RenderContext.h"
#include "RenderCamera.h"
#include "Graphics.h"

namespace Bolt
{

	struct BLT_API RenderOptions
	{
	public:
		const Framebuffer* RenderTarget = nullptr;
		RenderContext GlobalContext;
		RenderCameraRef CameraOverride;
	};

}