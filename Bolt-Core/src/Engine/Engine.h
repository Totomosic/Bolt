#pragma once
#include "Bolt-Graphics.h"
#include "Application.h"
#include "EngineCreateInfo.h"

namespace Bolt
{
	
	class BLT_API Engine
	{
	private:
		std::unique_ptr<Application> m_CurrentApplication;
		EngineCreateInfo m_CreateInfo;
		WindowCreateInfo m_WindowCreateInfo;
		bool m_ShouldExit;

	public:
		Engine(EngineCreateInfo createInfo = EngineCreateInfo());
		~Engine();

		bool ShouldClose() const;

		void UpdateApplication();
		void UpdateApplicationNoGraphics();
		void SetApplication(std::unique_ptr<Application>&& app);
		void SetWindowCreateInfo(const WindowCreateInfo& createInfo);
		void Run();

		template<typename T>
		void SetApplication()
		{
			SetApplication(std::make_unique<T>());
		}

	};

}