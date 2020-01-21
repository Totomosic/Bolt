#pragma once
#include "Engine.h"

namespace Bolt
{

	class BLT_API CurrentContext
	{
	public:
		CurrentContext() = delete;

		static AppContext& GetContext();
		static RenderContext& GetRenderContext();

		static Window& GetWindow();
	};

}