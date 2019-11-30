#include "bltpch.h"

#include "Engine.h"
#include "Application.h"
#include "Initialization/Initializer.h"
#include "Initialization/Destructor.h"
#include "User/Input.h"
#include "Renderer/Graphics.h"
#include "Renderer/GLState.h"
#include "Scene/SceneManager.h"

#include "Core/Profiling/Profiling.h"

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
		return m_ChildApps.size();
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

	void Application::CreateContext(bool createRenderContext, const WindowCreateInfo& createInfo)
	{
		BLT_PROFILE_FUNCTION();
		m_IsGraphicsEnabled = createRenderContext;
		m_Context = std::make_unique<AppContext>(createRenderContext, createInfo);
		Engine::Instance().SetCurrentContext(m_Context.get());
		if (createRenderContext)
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
		Scene* scene = &SceneManager::Get().CurrentScene();
		EventManager::Get().FlushAll(); // Flush #1 (likely input events)
		{
			BLT_PROFILE_SCOPE("Update()");
			Update();
		}
		if (scene != nullptr)
		{
			scene->Update();
		}
		{
			BLT_PROFILE_SCOPE("Render()");
			Render();
		}
		GetWindow().SwapBuffers();
		Time::Get().Update();
		EventManager::Get().FlushAll(); // Flush #2 (likely other scene/app events)
		if (scene != nullptr)
		{
			scene->UpdateTemporaryObjects();
		}
		return true;
	}

	bool Application::UpdateNoGraphics()
	{
		BLT_PROFILE_FUNCTION();
		{
			BLT_PROFILE_SCOPE("Update()");
			Update();
		}
		Time::Get().Update();
		EventManager::Get().FlushAll(); // Flush #2 (likely other scene/app events)
		return true;
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
		for (NewAppInfo& app : m_NewApps)
		{
			PushNewApp(app);
		}
		m_NewApps.clear();
	}

	void Application::PushNewApp(Application::NewAppInfo& app)
	{
		BLT_PROFILE_FUNCTION();
		Application* ptr = app.App.get();
		ptr->m_AppId = m_ChildApps.size() + 1;
		m_ChildApps.push_back(std::move(app.App));
		ptr->CreateContext(m_IsGraphicsEnabled, app.Info);
		ptr->Start();
	}

}