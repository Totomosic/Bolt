#include "Application.h"
#include "Initialization\Initializer.h"
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

	float Application::Width() const
	{
		return PrimaryWindow->Width();
	}

	float Application::Height() const
	{
		return PrimaryWindow->Height();
	}

	void Application::Start(Window* engineWindow)
	{
		SetPrimaryWindow(engineWindow);
		Init();
		BLT_INFO("Init took " + std::to_string(Time::CurrentTime()) + " seconds");
		Time::Reset();
		m_TickTimer = Time::CreateTimer(1.0, std::bind(&Application::Tick, this));
	}

	void Application::SetPrimaryWindow(Window* window)
	{
		PrimaryWindow = window;
		if (!Initializer::Run(PrimaryWindow))
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