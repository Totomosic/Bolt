#include "bltpch.h"
#include "AppContext.h"

#include "Engine.h"

namespace Bolt
{

	ContextManager::ContextManager(AppContext* context)
	{
		Engine::Instance().SetCurrentContext(context);
	}

	AppContext::AppContext(const WindowCreateInfo& createInfo)
		: m_Context(this), m_Time(), m_Events(), m_RenderContext(std::make_unique<RenderContext>(this, createInfo)), m_SceneManager()
	{
	}

	AppContext::AppContext()
		: m_Context(this), m_Time(), m_Events(), m_RenderContext(nullptr), m_SceneManager()
	{
	}

	bool AppContext::HasRenderContext() const
	{
		return m_RenderContext != nullptr;
	}

	const RenderContext& AppContext::GetRenderContext() const
	{
		return *m_RenderContext;
	}

	RenderContext& AppContext::GetRenderContext()
	{
		return *m_RenderContext;
	}

	EventManager& AppContext::GetEventManager()
	{
		return m_Events;
	}

	Time& AppContext::GetTime()
	{
		return m_Time;
	}

	SceneManager& AppContext::GetSceneManager()
	{
		return m_SceneManager;
	}

}