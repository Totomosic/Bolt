#pragma once
#include "RenderingContext.h"
#include "RenderCamera.h"
#include "Graphics.h"

namespace Bolt
{

	struct BLT_API RenderOptions
	{
	public:
		const Framebuffer* RenderTarget = nullptr;
		RenderingContext GlobalContext;
		RenderCameraRef CameraOverride;
	};

}