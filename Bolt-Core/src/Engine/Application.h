#pragma once
#include "Bolt-Graphics.h"

namespace Bolt
{

	class BLT_API Application
	{
	private:
		Timer* m_TickTimer;
		bool m_IsRunning;
		bool m_ShouldExit;

	public:
		std::unique_ptr<Window> AppWindow;

	public:
		float Width() const;
		float Height() const;

		void Start();

		virtual void Init();
		virtual void Tick();
		virtual void Update();
		virtual void Render();
		virtual void Exit();

		friend class Engine;

	private:
		void CreateWindowPtr(const WindowCreateInfo& createInfo);
		void ExitPrivate();

	};

}