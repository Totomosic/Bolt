#include "bltpch.h"

#include "Engine.h"
#include "Initialization/Initializer.h"
#include "User/Input.h"
#include "Renderer/Graphics.h"
#include "Renderer/GLState.h"
#include "Engine/Initialization/Destructor.h"

#include "BoltLib/Profiling/Profiling.h"

namespace Bolt
{

	Engine* Engine::s_EngineInstance = nullptr;

	Engine& Engine::Instance()
	{
		BLT_ASSERT(s_EngineInstance != nullptr, "Engine has not been created yet");
		return *s_EngineInstance;
	}

	bool Engine::IsInitialized()
	{
		return s_EngineInstance != nullptr;
	}

	Engine::Engine(EngineCreateInfo createInfo)
		: m_RootApplication(), m_CreateInfo(createInfo), m_CurrentContext(nullptr), m_ImGuiInitialized(false)
	{
		BLT_PROFILE_BEGIN_SESSION("BoltInit", "BoltInit.json");
		s_EngineInstance = this;
		Initializer::PreOpenGL(m_CreateInfo);
	}

	Engine::~Engine()
	{
		BLT_PROFILE_BEGIN_SESSION("BoltDestroy", "BoltDestroy.json");
		{
			BLT_PROFILE_FUNCTION();
			if (m_ImGuiInitialized)
			{
				ShutdownImGui();
				m_ImGuiInitialized = false;
			}
			m_RootApplication.reset(nullptr);
			Destructor::Run();
		}
		BLT_PROFILE_END_SESSION();
	}

	AppContext& Engine::CurrentContext() const
	{
		return *m_CurrentContext;
	}

	void Engine::UpdateApplication()
	{
		BLT_PROFILE_FUNCTION();
		m_RootApplication->UpdateInput();
		m_RootApplication->UpdatePrivate();
	}

	void Engine::Run()
	{
		BLT_PROFILE_END_SESSION();
		BLT_ASSERT(m_RootApplication.get() != nullptr, "Must have a valid Application to run");
		BLT_PROFILE_BEGIN_SESSION("BoltRuntime", "BoltRuntime.json");
		{
			BLT_PROFILE_FUNCTION();
			while (!m_RootApplication->ShouldExit())
			{
				UpdateApplication();
			}
		}
		BLT_PROFILE_END_SESSION();
	}

	void Engine::SetCurrentContext(AppContext* context)
	{
		m_CurrentContext = context;
	}

	void Engine::ApplyCurrentContext(AppContext* context)
	{
		BLT_PROFILE_FUNCTION();
		if (m_CurrentContext != context)
		{
			SetCurrentContext(context);
			if (context->HasRenderContext())
			{
				context->GetRenderContext().GetWindow().MakeCurrent();
			}
		}
	}

}