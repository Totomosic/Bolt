#include "Types.h"

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
		AppWindow.release();
	}

	float Application::Width() const
	{
		return AppWindow->Width();
	}

	float Application::Height() const
	{
		return AppWindow->Height();
	}

	void Application::Start()
	{
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

	void Application::CreateWindowPtr(const WindowCreateInfo& createInfo)
	{
		AppWindow = std::make_unique<Window>(createInfo);
	}

}