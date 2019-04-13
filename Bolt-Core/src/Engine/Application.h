#pragma once
#include "EngineCreateInfo.h"
#include "AppContext.h"

namespace Bolt
{

	class BLT_API Application
	{
	public:
		struct BLT_API NewAppInfo
		{
		public:
			std::unique_ptr<Application> App;
			WindowCreateInfo Info;

		};

	private:
		Timer* m_TickTimer;
		bool m_IsRunning;
		bool m_ShouldExit;
		std::unique_ptr<AppContext> m_Context;
		std::vector<std::unique_ptr<Application>> m_ChildApps;
		std::vector<NewAppInfo> m_NewApps;

	public:
		const AppContext& GetContext() const;
		AppContext& GetContext();
		//const Window& GetWindow() const;
		Window& GetWindow();

		float Width() const;
		float Height() const;

		void Start();

		template<typename T>
		void PushApp(const WindowCreateInfo& info = WindowCreateInfo())
		{
			m_NewApps.push_back({ std::make_unique<T>(), info });
		}

		virtual void Init();
		virtual void Tick();
		virtual void Update();
		virtual void Render();
		virtual void Exit();

		friend class Engine;

	private:
		void CreateContext(const WindowCreateInfo& createInfo);
		void ExitPrivate();
		bool UpdatePrivate();

		void UpdateInput();

		void PushNewApps();
		void PushNewApp(NewAppInfo& app);

	};

}