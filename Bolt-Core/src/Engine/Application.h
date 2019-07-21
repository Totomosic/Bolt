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
		bool m_IsGraphicsEnabled;

		Timer* m_TickTimer;
		bool m_IsRunning;
		bool m_ShouldExit;
		std::unique_ptr<AppContext> m_Context;
		std::vector<std::unique_ptr<Application>> m_ChildApps;
		std::vector<NewAppInfo> m_NewApps;

	public:
		Application();
		virtual ~Application();

		const AppContext& GetContext() const;
		AppContext& GetContext();
		//const Window& GetWindow() const;
		Window& GetWindow();
		Application& GetChildApp(int index) const;
		int ChildCount() const;

		float Width() const;
		float Height() const;
		bool ShouldExit() const;

		void Start();

		template<typename T>
		void PushApp(const WindowCreateInfo& info = WindowCreateInfo())
		{
			m_NewApps.push_back(NewAppInfo{ std::make_unique<T>(), info });
		}

		template<typename T>
		T& GetChildApp(int index) const
		{
			return (T&)GetChildApp(index);
		}

		virtual void Init();
		virtual void Tick();
		virtual void Update();
		virtual void Render();
		virtual void Exit();

		friend class Engine;

	private:
		void CreateContext(bool createRenderContext, const WindowCreateInfo& createInfo);
		bool UpdatePrivate();
		bool UpdateGraphics();
		bool UpdateNoGraphics();

		void CloseChild(int index);

		void UpdateInput();

		void PushNewApps();
		void PushNewApp(NewAppInfo& app);

	};

}