#include "Application.h"
#include "Initialization\Initializor.h"
#include "User\Input.h"
#include "Renderer\Graphics.h"
#include "Renderer\GLState.h"
#include "Scene\SceneManager.h"

namespace Bolt
{

	Application::~Application()
	{
		ImGui_ImplGlfwGL3_Shutdown();
		ImGui::DestroyContext();
	}

	void Application::Start(Window* engineWindow)
	{
		SetPrimaryWindow(engineWindow);
		Init();
		Time::Reset();
		m_TickTimer = Time::CreateTimer(1.0f, std::bind(&Application::Tick, this));
	}

	void Application::SetPrimaryWindow(Window* window)
	{
		PrimaryWindow = window;
		if (!Initializor::Run(PrimaryWindow))
		{
			BLT_ASSERT(false, "Failed to initialize");
		}
		SceneManager::s_Window = PrimaryWindow;
		GLState::Reset();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplGlfwGL3_Init(PrimaryWindow->WindowHandle(), false);
		ImGui::StyleColorsDark();
	}

	void Application::Init()
	{
	
	}

	void Application::Tick()
	{
		
	}

	void Application::Update()
	{
	
	}

	void Application::ImGuiRender()
	{
	
	}

	void Application::Render()
	{
	
	}

}