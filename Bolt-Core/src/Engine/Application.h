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
		bool m_UseImGui;
		id_t m_AppId;

		Timer* m_TickTimer;
		bool m_IsRunning;
		bool m_ShouldExit;
		std::unique_ptr<AppContext> m_Context;
		std::vector<std::unique_ptr<Application>> m_ChildApps;
		std::vector<std::unique_ptr<Application>> m_NewApps;

	public:
		Application();
		virtual ~Application();

		const AppContext& GetContext() const;
		AppContext& GetContext();
		Window& GetWindow();
		Application& GetChildApp(int index) const;
		int ChildCount() const;
		id_t GetAppId() const;
		bool IsChildApp() const;

		float Width() const;
		float Height() const;
		bool ShouldExit() const;

		void Start();

		template<typename T, typename ... Args>
		void PushApp(const WindowCreateInfo& info, Args&& ... args)
		{
			AppContext* currentContext = GetCurrentContext();
			std::unique_ptr<AppContext> context = std::make_unique<AppContext>(info);
			SetActiveContext(context.get());
			std::unique_ptr<T> app = std::make_unique<T>(std::forward<Args>(args)...);
			app->SetUseImGui(m_UseImGui);
			app->SetContext(std::move(context));
			app->m_AppId = m_AppId + m_ChildApps.size() + m_NewApps.size() + 1;
			m_NewApps.push_back(std::move(app));
			SetActiveContext(currentContext);
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
		AppContext* GetCurrentContext() const;
		void SetActiveContext(AppContext* context) const;
		void SetContext(std::unique_ptr<AppContext>&& context);
		bool UpdatePrivate();
		bool UpdateGraphics();
		bool UpdateNoGraphics();

		void BeforeRender();
		void AfterRender();

		void CloseChild(int index);
		void UpdateInput();
		void PushNewApps();

		void SetUseImGui(bool use);

	};

}