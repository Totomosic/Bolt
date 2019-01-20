#pragma once
#include "Bolt-Graphics.h"
#include "Application.h"

namespace Bolt
{
	
	class BLT_API Engine
	{
	private:
		std::unique_ptr<Application> m_CurrentApplication;
		WindowCreateInfo m_CreateInfo;
		bool m_ShouldExit;

	public:
		Engine();
		~Engine();

		bool ShouldClose() const;

		void UpdateApplication();
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