#include "bltpch.h"

#include "Graphics.h"
#include "Engine/Engine.h"

namespace Bolt
{

	Graphics& Graphics::Get()
	{
		return Engine::Instance().CurrentContext().GetRenderContext().GetGraphics();
	}

	Graphics::Graphics(Window* window)
		: m_Window(window), m_State(), m_Renderer()
	{
	
	}

	const Framebuffer* Graphics::DefaultFramebuffer()
	{
		return &m_Window->GetFramebuffer();
	}	

	const GLState& Graphics::GetState() const
	{
		return m_State;
	}

	GLState& Graphics::GetState()
	{
		return m_State;
	}

	const Renderer& Graphics::GetRenderer() const
	{
		return m_Renderer;
	}

	Renderer& Graphics::GetRenderer()
	{
		return m_Renderer;
	}

	void Graphics::RenderScene()
	{
		DefaultFramebuffer()->Clear();
		if (SceneManager::Get().HasCurrentScene())
		{
			TimeDelta delta = Time::Get().RenderingTimeline().DeltaTime();
			SceneManager::Get().GetCurrentScene().Render(delta);
		}
	}

}