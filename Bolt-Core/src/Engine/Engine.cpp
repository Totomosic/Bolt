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
		Filesystem::Initialize();
		int result = glfwInit();
		BLT_ASSERT(result != GL_NO_ERROR, "GLFW failed to initialise");
		Random::Initialize();
		WSADATA data;
		if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
		{
			BLT_ASSERT(false, "Failed to initalise WinSock2");
		}
	}

	Engine::~Engine()
	{
		SceneManager::Terminate();
		ResourceManager::Terminate();
		Input::Terminate();
		WSACleanup();
		m_Window.release(); // TEMPORARY
	}

	bool Engine::ShouldClose() const
	{
		return m_Window->ShouldClose();
	}

	void Engine::SetWindow(std::unique_ptr<Window>&& window)
	{
		m_Window = std::move(window);
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
		m_Window->SwapBuffers();
		Time::Update();
		EventManager::FlushEvents(); // Flush #2 (likely other scene/app events)
		if (scene != nullptr)
		{
			scene->UpdateTemporaryObjects();
		}
	}

	void Engine::SetApplication(std::unique_ptr<Application>&& app)
	{
		SceneManager::Terminate();
		ResourceManager::Terminate();
		Input::Terminate();
		Time::Reset();
		m_CurrentApplication = std::move(app);
		m_CurrentApplication->Start(m_Window.get());
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