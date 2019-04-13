#pragma once
#include "Bolt-Graphics.h"
#include "Scene/__Scene__.h"
#include "Renderer/__Renderer__.h"
#include "User/__User__.h"

namespace Bolt
{

	class BLT_API RenderContext
	{
	private:
		EventManager m_Events;
		Window m_Window;
		Graphics m_Graphics;
		SceneManager m_SceneManager;
		SceneRenderer m_SceneRenderer;
		Input m_Input;
		ResourceManager m_Resources;

	};

}