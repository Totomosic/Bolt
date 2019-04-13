#include "Types.h"

#include "Graphics.h"
#include "SceneRenderer.h"

namespace Bolt
{

	std::unique_ptr<Graphics> Graphics::s_Instance;

	Graphics& Graphics::Get()
	{
		if (!s_Instance)
		{
			s_Instance = std::make_unique<Graphics>();
		}
		return *s_Instance;
	}

	Graphics::Graphics()
		: m_Window(nullptr), m_State()
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

	void Graphics::Initialize(Window* window)
	{
		Get().m_Window = window;
		Get().GetState().Reset();
	}

	void Graphics::Terminate()
	{
		Get().m_Window = nullptr;
	}

}