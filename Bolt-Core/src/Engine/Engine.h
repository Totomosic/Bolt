#pragma once
#include "Application.h"
#include "EngineCreateInfo.h"
#include "AppContext.h"
#include "ImGui/ImGui.h"

namespace Bolt
{
	
	class BLT_API Engine
	{
	private:
		static Engine* s_EngineInstance;

	private:
		std::unique_ptr<Application> m_RootApplication;
		EngineCreateInfo m_CreateInfo;

		AppContext* m_CurrentContext;
		bool m_ImGuiInitialized;

	public:
		static Engine& Instance();
		static bool IsInitialized();

	public:
		Engine(EngineCreateInfo createInfo = EngineCreateInfo());
		Engine(Engine&& other) = delete;
		Engine& operator=(Engine&& other) = delete;
		~Engine();

		AppContext& CurrentContext() const;

		void UpdateApplication();
		void Run();

		template<typename T, typename ... Args>
		void SetApplication(Args&& ... args)
		{
			BLT_PROFILE_FUNCTION();
			std::unique_ptr<AppContext> context;
			if (m_CreateInfo.UseGraphics)
				context = std::make_unique<AppContext>(m_CreateInfo.WindowInfo);
			else
				context = std::make_unique<AppContext>();
			ApplyCurrentContext(context.get());
			m_RootApplication = std::make_unique<T>(std::forward<Args>(args)...);

			if (!m_ImGuiInitialized && m_CreateInfo.UseGraphics && m_CreateInfo.EnableImGui && context->HasRenderContext())
				m_ImGuiInitialized = InitImGui(context->GetRenderContext().GetWindow());
			
			m_RootApplication->SetUseImGui(m_ImGuiInitialized);
			m_RootApplication->SetContext(std::move(context));
			m_RootApplication->Start();
		}

		friend class Application;
		friend class ContextManager;

	private:
		void SetCurrentContext(AppContext* context);
		void ApplyCurrentContext(AppContext* context);

	};

}