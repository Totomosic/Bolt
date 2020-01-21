#pragma once
#include "bltpch.h"

namespace Bolt
{

	struct BLT_API RendererStats
	{
	public:
		int SceneCount;
		int RenderGroupCount;
		int DrawCalls;
	};

}