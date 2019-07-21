#pragma once
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

	class BLT_API RenderContext
	{
	private:
		Input m_Input;
		Window m_Window;
		ResourceManager m_Resources;
		BasicModels m_Models;
		Graphics m_Graphics;
		SceneManager m_SceneManager;
		SceneRenderer m_SceneRenderer;	

	public:
		RenderContext(AppContext* context, const WindowCreateInfo& createInfo);

		Window& GetWindow();
		BasicModels& GetBasicModels();
		Graphics& GetGraphics();
		SceneManager& GetSceneManager();
		SceneRenderer& GetSceneRenderer();
		Input& GetInput();
		ResourceManager& GetResourceManager();

	};

}