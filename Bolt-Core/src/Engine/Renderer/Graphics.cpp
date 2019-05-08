#include "Types.h"

#include "Graphics.h"
#include "SceneRenderer.h"
#include "Engine/Engine.h"

namespace Bolt
{

	Graphics& Graphics::Get()
	{
		return Engine::Instance().CurrentContext().GetRenderContext().GetGraphics();
	}

	Graphics::Graphics(Window* window)
		: m_Window(window), m_State()
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

	void Graphics::RenderScene()
	{
		if (&SceneManager::Get().CurrentScene() == nullptr)
		{
			BLT_CORE_WARN("No Active Scene");
			return;
		}
		DefaultFramebuffer()->Clear();
		SceneRenderer::Get().Render(SceneManager::Get().CurrentScene());
	}

}