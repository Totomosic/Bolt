#include "Types.h"

#include "Engine.h"
#include "Application.h"
#include "Initialization\Initializer.h"
#include "Initialization\Destructor.h"
#include "User\Input.h"
#include "Renderer\Graphics.h"
#include "Renderer\GLState.h"
#include "Scene\SceneManager.h"

namespace Bolt
{

	Application::Application()
		: m_TickTimer(nullptr), m_IsRunning(false), m_ShouldExit(false), m_Context(), m_ChildApps(), m_NewApps()
	{
		
	}

	Application::~Application()
	{
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

	float Application::Width() const
	{
		return (float)m_Context->GetRenderContext().GetWindow().Width();
	}

	float Application::Height() const
	{
		return (float)m_Context->GetRenderContext().GetWindow().Height();
	}

	void Application::Start()
	{
		m_IsRunning = true;
		m_ShouldExit = false;
		Init();
		Time::Update();
		BLT_CORE_INFO("Init took " + std::to_string(Time::RenderingTimeline().CurrentRealTime()) + " seconds");
		Time::Reset();
		m_TickTimer = &Time::RenderingTimeline().AddTimer(1.0, std::bind(&Application::Tick, this));
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

	void Application::CreateContext(const WindowCreateInfo& createInfo)
	{
		m_Context = std::make_unique<AppContext>(createInfo);
		Engine::Instance().SetCurrentContext(m_Context.get());
	}

	void Application::ExitPrivate()
	{
		m_IsRunning = false;
	}

	bool Application::UpdatePrivate()
	{
		PushNewApps();
		UpdateInput();
		for (std::unique_ptr<Application>& child : m_ChildApps)
		{
			child->UpdatePrivate();
		}
		Engine::Instance().ApplyCurrentContext(m_Context.get());
		Scene* scene = &SceneManager::Get().CurrentScene();
		EventManager::Get().FlushEvents(); // Flush #1 (likely input events)
		Update();
		if (scene != nullptr)
		{
			scene->Update();
		}
		Render();
		GetWindow().SwapBuffers();
		Time::Update();
		EventManager::Get().FlushEvents(); // Flush #2 (likely other scene/app events)
		if (scene != nullptr)
		{
			scene->UpdateTemporaryObjects();
		}
		if (m_ShouldExit)
		{
			ExitPrivate();
		}
		return true;
	}

	void Application::CloseChild(int index)
	{
		Application* ptr = m_ChildApps.at(index).get();
		Engine::Instance().ApplyCurrentContext(&ptr->GetContext());
		m_ChildApps.erase(m_ChildApps.begin() + index);
	}

	void Application::UpdateInput()
	{
		GetContext().GetRenderContext().GetInput().Update();
		for (std::unique_ptr<Application>& child : m_ChildApps)
		{
			child->GetContext().GetRenderContext().GetInput().Update();
		}
		glfwPollEvents();
	}

	void Application::PushNewApps()
	{
		for (NewAppInfo& app : m_NewApps)
		{
			PushNewApp(app);
		}
		m_NewApps.clear();
	}

	void Application::PushNewApp(Application::NewAppInfo& app)
	{
		Application* ptr = app.App.get();
		m_ChildApps.push_back(std::move(app.App));
		ptr->CreateContext(app.Info);
		ptr->Start();
	}

}