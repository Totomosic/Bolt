#include "Types.h"

#include "Engine.h"
#include "Initialization\Initializer.h"
#include "User\Input.h"
#include "Renderer\Graphics.h"
#include "Renderer\GLState.h"
#include "Scene\SceneManager.h"
#include "Engine/Initialization/Destructor.h"

namespace Bolt
{

	Engine* Engine::s_EngineInstance = nullptr;

	Engine& Engine::Instance()
	{
		BLT_ASSERT(s_EngineInstance != nullptr, "Engine has not been created yet");
		return *s_EngineInstance;
	}

	Engine::Engine(EngineCreateInfo createInfo)
		: m_RootApplication(), m_CreateInfo(createInfo), m_ShouldExit(false), m_CurrentContext(nullptr)
	{
		s_EngineInstance = this;
		Initializer::PreOpenGL(m_CreateInfo);
	}

	Engine::~Engine()
	{
		m_RootApplication.reset(nullptr);
		Destructor::Run();
	}

	AppContext& Engine::CurrentContext() const
	{
		return *m_CurrentContext;
	}

	bool Engine::ShouldClose() const
	{
		return m_RootApplication->GetContext().GetRenderContext().GetWindow().ShouldClose();
	}

	void Engine::UpdateApplication()
	{
		m_RootApplication->UpdatePrivate();
	}

	void Engine::SetApplication(std::unique_ptr<Application>&& app)
	{
		m_RootApplication = std::move(app);
		m_RootApplication->CreateContext(m_CreateInfo.WindowInfo);
		m_RootApplication->Start();
	}

	void Engine::Run()
	{
		BLT_ASSERT(m_RootApplication.get() != nullptr, "Must have a valid Application to run");
		m_RootApplication->GetWindow().OnClose().Subscribe([this](WindowClosedEvent& e)
		{
			m_ShouldExit = true;
			ListenerResponse response;
			response.HandledEvent = false;
			return response;
		});
		while (m_RootApplication->m_IsRunning)
		{
			UpdateApplication();
			if (m_ShouldExit)
			{
				m_RootApplication->Exit();
				m_ShouldExit = false;
			}
		}
	}

	void Engine::SetCurrentContext(AppContext* context)
	{
		m_CurrentContext = context;
	}

}