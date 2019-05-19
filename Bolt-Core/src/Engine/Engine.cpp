#include "bltpch.h"

#include "Engine.h"
#include "Initialization/Initializer.h"
#include "User/Input.h"
#include "Renderer/Graphics.h"
#include "Renderer/GLState.h"
#include "Scene/SceneManager.h"
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
		: m_RootApplication(), m_CreateInfo(createInfo), m_CurrentContext(nullptr)
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

	void Engine::UpdateApplication()
	{
		m_RootApplication->UpdateInput();
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
		while (!m_RootApplication->ShouldExit())
		{
			UpdateApplication();
		}
	}

	void Engine::SetCurrentContext(AppContext* context)
	{
		m_CurrentContext = context;
	}

	void Engine::ApplyCurrentContext(AppContext* context)
	{
		SetCurrentContext(context);
		context->GetRenderContext().GetWindow().MakeCurrent();
	}

}