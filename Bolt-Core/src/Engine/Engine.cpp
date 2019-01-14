#include "Types.h"

#include "Engine.h"
#include "Initialization\Initializer.h"
#include "User\Input.h"
#include "Renderer\Graphics.h"
#include "Renderer\GLState.h"
#include "Scene\SceneManager.h"

namespace Bolt
{

	Engine::Engine()
	{
		Initializer::PreOpenGL();
	}

	Engine::~Engine()
	{
		SceneManager::Terminate();
		ResourceManager::Terminate();
		Input::Terminate();
		WSACleanup();
	}

	bool Engine::ShouldClose() const
	{
		return m_CurrentApplication->AppWindow->ShouldClose();
	}

	void Engine::UpdateApplication()
	{
		Scene* scene = &SceneManager::CurrentScene();
		Input::Update();
		glfwPollEvents();
		EventManager::FlushEvents(); // Flush #1 (likely input events)
		m_CurrentApplication->Update();
		if (scene != nullptr)
		{
			scene->Update();
		}
		m_CurrentApplication->Render();
		m_CurrentApplication->AppWindow->SwapBuffers();
		Time::Update();
		EventManager::FlushEvents(); // Flush #2 (likely other scene/app events)
		if (scene != nullptr)
		{
			scene->UpdateTemporaryObjects();
		}
	}

	void Engine::SetApplication(std::unique_ptr<Application>&& app)
	{
		m_CurrentApplication = std::move(app);
		m_CurrentApplication->CreateWindowPtr(m_CreateInfo);
		Initializer::PostOpenGL(m_CurrentApplication->AppWindow.get());
		m_CurrentApplication->Start();
	}

	void Engine::SetWindowCreateInfo(const WindowCreateInfo& createInfo)
	{
		m_CreateInfo = createInfo;
	}

	void Engine::Run()
	{
		BLT_ASSERT(m_CurrentApplication.get() != nullptr, "Must have a valid Application to run");
		while (!ShouldClose())
		{
			UpdateApplication();
		}
	}

}