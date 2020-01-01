#include "bltpch.h"
#include "RenderContext.h"
#include "AppContext.h"
#include "Engine/Engine.h"

namespace Bolt
{

	RenderContext::RenderContext(AppContext* context, const WindowCreateInfo& createInfo)
		: m_Input(&m_Window), m_Window(context, createInfo), m_Resources(), m_Graphics(&m_Window)
	{

	}

	Window& RenderContext::GetWindow()
	{
		return m_Window;
	}

	Graphics& RenderContext::GetGraphics()
	{
		return m_Graphics;
	}

	Input& RenderContext::GetInput()
	{
		return m_Input;
	}

	ResourceManager& RenderContext::GetResourceManager()
	{
		return m_Resources;
	}

}