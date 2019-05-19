#include "bltpch.h"
#include "RenderContext.h"
#include "AppContext.h"
#include "Engine/Engine.h"

namespace Bolt
{

	ContextManager::ContextManager(AppContext* context)
	{
		Engine::Instance().SetCurrentContext(context);
	}

	RenderContext::RenderContext(AppContext* context, const WindowCreateInfo& createInfo)
		: m_Context(context), m_Events(), m_Time(), m_Input(&m_Window), m_Window(context, createInfo), m_Resources(), m_Models(), m_Graphics(&m_Window), m_SceneManager(), m_SceneRenderer()
	{

	}

	RenderContext::~RenderContext()
	{
		
	}

	EventManager& RenderContext::GetEventManager()
	{
		return m_Events;
	}

	Time& RenderContext::GetTime()
	{
		return m_Time;
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