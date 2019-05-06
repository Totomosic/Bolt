#pragma once
#include "Core/Time/Time.h"
#include "Core/Events/EventManager.h"
#include "Engine/User/Input.h"
#include "Graphics/Window.h"
#include "Graphics/Resources/ResourceManager.h"
#include "Engine/Scene/BasicModels.h"
#include "Engine/Renderer/Graphics.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Renderer/SceneRenderer.h"

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