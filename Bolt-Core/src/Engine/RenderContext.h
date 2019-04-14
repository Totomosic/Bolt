#pragma once
#include "Bolt-Graphics.h"
#include "Scene/__Scene__.h"
#include "Renderer/__Renderer__.h"
#include "User/__User__.h"

namespace Bolt
{

	class AppContext;

	class BLT_API ContextManager
	{
	public:
		ContextManager(AppContext* context);
	};

	class BLT_API RenderContext
	{
	private:
		ContextManager m_Context;
		Time m_Time;
		EventManager m_Events;
		Input m_Input;
		Window m_Window;
		ResourceManager m_Resources;
		BasicModels m_Models;
		Graphics m_Graphics;
		SceneManager m_SceneManager;
		SceneRenderer m_SceneRenderer;	

	public:
		RenderContext(AppContext* context, const WindowCreateInfo& createInfo);
		~RenderContext();

		EventManager& GetEventManager();
		Time& GetTime();
		Window& GetWindow();
		BasicModels& GetBasicModels();
		Graphics& GetGraphics();
		SceneManager& GetSceneManager();
		SceneRenderer& GetSceneRenderer();
		Input& GetInput();
		ResourceManager& GetResourceManager();

	};

}