#include "bltpch.h"
#include "CurrentContext.h"

namespace Bolt
{

	AppContext& CurrentContext::GetContext()
	{
		return Engine::Instance().CurrentContext();
	}

	RenderContext& CurrentContext::GetRenderContext()
	{
		if (GetContext().HasRenderContext())
		{
			return GetContext().GetRenderContext();
		}
		return *(RenderContext*)nullptr;
	}

	Window& CurrentContext::GetWindow()
	{
		if (&GetRenderContext() != nullptr)
		{
			return GetRenderContext().GetWindow();
		}
		return *(Window*)nullptr;
	}

}