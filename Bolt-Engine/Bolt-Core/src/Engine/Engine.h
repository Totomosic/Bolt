#pragma once
#include "Application.h"
#include "EngineCreateInfo.h"
#include "AppContext.h"

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

	public:
		static Engine& Instance();

	public:
		Engine(EngineCreateInfo createInfo = EngineCreateInfo());
		Engine(Engine&& other) = delete;
		Engine& operator=(Engine&& other) = delete;
		~Engine();

		AppContext& CurrentContext() const;

		void UpdateApplication();
		void SetApplication(std::unique_ptr<Application>&& app);
		void Run();

		template<typename T>
		void SetApplication()
		{
			SetApplication(std::make_unique<T>());
		}

		friend class Application;
		friend class ContextManager;

	private:
		void SetCurrentContext(AppContext* context);
		void ApplyCurrentContext(AppContext* context);

	};

}