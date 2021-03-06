#pragma once
#include "Engine/User/Input.h"
#include "Graphics/Window.h"
#include "Graphics/Assets/AssetManager.h"
#include "Engine/Renderer/Graphics.h"
#include "Engine/Scene/SceneManager.h"

namespace Bolt
{

	class AppContext;

	class BLT_API RenderContext
	{
	private:
		Input m_Input;
		Window m_Window;
		AssetManager m_Resources;
		Graphics m_Graphics;

	public:
		RenderContext(AppContext* context, const WindowCreateInfo& createInfo);

		Window& GetWindow();
		Graphics& GetGraphics();
		Input& GetInput();
		AssetManager& GetAssetManager();

	};

}