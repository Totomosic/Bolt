#include "bltpch.h"

#include "Engine.h"
#include "Application.h"
#include "Initialization/Initializer.h"
#include "Initialization/Destructor.h"
#include "User/Input.h"
#include "Renderer/Graphics.h"
#include "Renderer/GLState.h"

#include "BoltLib/Profiling/Profiling.h"

#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

namespace Bolt
{

	Application::Application()
		: m_IsGraphicsEnabled(true), m_AppId(0), m_TickTimer(nullptr), m_IsRunning(false), m_ShouldExit(false), m_Context(), m_ChildApps(), m_NewApps()
	{
		
	}

	Application::~Application()
	{
		BLT_PROFILE_FUNCTION();
		for (int i = m_ChildApps.size() - 1; i >= 0; i--)
		{
			CloseChild(i);
		}
		Engine::Instance().ApplyCurrentContext(m_Context.get());
	}

	const AppContext& Application::GetContext() const
	{
		return *m_Context;
	}

	AppContext& Application::GetContext()
	{
		return *m_Context;
	}

	Window& Application::GetWindow()
	{
		return GetContext().GetRenderContext().GetWindow();
	}

	Application& Application::GetChildApp(int index) const
	{
		return *m_ChildApps.at(index);
	}

	int Application::ChildCount() const
	{
		return (int)m_ChildApps.size();
	}

	id_t Application::GetAppId() const
	{
		return m_AppId;
	}

	bool Application::IsChildApp() const
	{
		return m_AppId != 0;
	}

	float Application::Width() const
	{
		return (float)m_Context->GetRenderContext().GetWindow().Width();
	}

	float Application::Height() const
	{
		return (float)m_Context->GetRenderContext().GetWindow().Height();
	}

	bool Application::ShouldExit() const
	{
		return m_ShouldExit;
	}

	void Application::Start()
	{
		BLT_PROFILE_FUNCTION();
		m_IsRunning = true;
		m_ShouldExit = false;
		if (!m_IsGraphicsEnabled)
		{
			BLT_CORE_WARN("Graphics are disabled");
		}
		BLT_CORE_INFO("Initializing...");
		{		
			BLT_PROFILE_SCOPE("Init()");
			Init();
		}
		Time::Get().Update();
		BLT_CORE_INFO("Init took " + std::to_string(Time::Get().RenderingTimeline().CurrentRealTime()) + " seconds");
		Time::Get().Reset();
		m_TickTimer = &Time::Get().RenderingTimeline().AddTimer(1.0, std::bind(&Application::Tick, this));
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

	void Application::Render()
	{
	
	}

	void Application::Exit()
	{
		m_ShouldExit = true;
	}

	AppContext* Application::GetCurrentContext() const
	{
		return &Engine::Instance().CurrentContext();
	}

	void Application::SetActiveContext(AppContext* context) const
	{
		Engine::Instance().ApplyCurrentContext(context);
	}

	void Application::SetContext(std::unique_ptr<AppContext>&& context)
	{
		BLT_PROFILE_FUNCTION();
		m_IsGraphicsEnabled = context->HasRenderContext();
		m_Context = std::move(context);
		if (m_IsGraphicsEnabled)
		{
			m_Context->GetRenderContext().GetWindow().OnClose().AddEventListener([this](Event<WindowClosedEvent>& e)
				{
					Exit();
				});
		}
	}

	bool Application::UpdatePrivate()
	{
		BLT_PROFILE_FUNCTION();
		PushNewApps();
		for (int i = m_ChildApps.size() - 1; i >=0; i--)
		{
			std::unique_ptr<Application>& child = m_ChildApps.at(i);
			child->UpdatePrivate();
			if (child->ShouldExit())
			{
				CloseChild(i);
			}
		}
		Engine::Instance().ApplyCurrentContext(m_Context.get());
		if (m_IsGraphicsEnabled)
		{
			return UpdateGraphics();
		}
		return UpdateNoGraphics();
	}

	bool Application::UpdateGraphics()
	{
		BLT_PROFILE_FUNCTION();
		EventManager::Get().FlushAll(); // Flush #1 (likely input events)
		{
			BLT_PROFILE_SCOPE("Update()");
			Update();
		}
		if (SceneManager::Get().HasCurrentScene())
		{
			TimeDelta delta = Time::Get().RenderingTimeline().DeltaTime();
			SceneManager::Get().GetCurrentScene().Update(delta);
		}
		BeforeRender();
		{
			BLT_PROFILE_SCOPE("Render()");
			Render();
			Graphics::Get().GetRenderer().Flush();
		}
		AfterRender();
		Time::Get().Update();
		EventManager::Get().FlushAll(); // Flush #2 (likely other scene/app events)
		return true;
	}

	bool Application::UpdateNoGraphics()
	{
		EventManager::Get().FlushAll();
		BLT_PROFILE_FUNCTION();
		{
			BLT_PROFILE_SCOPE("Update()");
			Update();
		}
		if (SceneManager::Get().HasCurrentScene())
		{
			TimeDelta delta = Time::Get().RenderingTimeline().DeltaTime();
			SceneManager::Get().GetCurrentScene().Update(delta);
		}
		Time::Get().Update();
		return true;
	}

	void Application::BeforeRender()
	{
		if (m_UseImGui)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}
	}

	void Application::AfterRender()
	{
		if (m_UseImGui)
		{
			Graphics::Get().DefaultFramebuffer()->Bind();
			ImGuiIO& io = ImGui::GetIO();
			io.DisplaySize = ImVec2((float)GetWindow().Width(), (float)GetWindow().Height());

			// Rendering
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* currentContext = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(currentContext);
			}
		}
		GetWindow().SwapBuffers();
	}

	void Application::CloseChild(int index)
	{
		BLT_PROFILE_FUNCTION();
		Application* ptr = m_ChildApps.at(index).get();
		Engine::Instance().ApplyCurrentContext(&ptr->GetContext());
		m_ChildApps.erase(m_ChildApps.begin() + index);
	}

	void Application::UpdateInput()
	{
		BLT_PROFILE_FUNCTION();
		if (GetContext().HasRenderContext())
		{
			GetContext().GetRenderContext().GetInput().Update();
			for (std::unique_ptr<Application>& child : m_ChildApps)
			{
				child->GetContext().GetRenderContext().GetInput().Update();
			}
			glfwPollEvents();
		}
	}

	void Application::PushNewApps()
	{
		BLT_PROFILE_FUNCTION();
		for (std::unique_ptr<Application>& app : m_NewApps)
		{
			Engine::Instance().ApplyCurrentContext(&app->GetContext());
			app->Start();
			m_ChildApps.push_back(std::move(app));
		}
		m_NewApps.clear();
	}

	void Application::SetUseImGui(bool use)
	{
		m_UseImGui = use;
	}

}