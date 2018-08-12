#pragma once
#include "Bolt-Graphics.h"
#include "Application.h"

namespace Bolt
{
	
	class BLT_API Engine
	{
	private:
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<Application> m_CurrentApplication;

	public:
		Engine();
		~Engine();

		bool ShouldClose() const;

		void SetWindow(std::unique_ptr<Window>&& window);
		void UpdateApplication();
		void SetApplication(std::unique_ptr<Application>&& app);

	};

}