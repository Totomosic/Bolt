#include "bltpch.h"
#include "RenderContext.h"
#include "AppContext.h"
#include "Engine/Engine.h"

namespace Bolt
{

	RenderContext::RenderContext(AppContext* context, const WindowCreateInfo& createInfo)
		: m_Input(&m_Window), m_Window(context, createInfo), m_Resources(), m_Models(&m_Resources), m_Graphics(&m_Window), m_SceneManager(), m_SceneRenderer()
	{

	}

	Window& RenderContext::GetWindow()
	{
		return m_Window;
	}

	BasicModels& RenderContext::GetBasicModels()
	{
		return m_Models;
	}

	Graphics& RenderContext::GetGraphics()
	{
		return m_Graphics;
	}

	SceneManager& RenderContext::GetSceneManager()
	{
		return m_SceneManager;
	}

	SceneRenderer& RenderContext::GetSceneRenderer()
	{
		return m_SceneRenderer;
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