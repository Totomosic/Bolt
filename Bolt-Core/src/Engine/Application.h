#pragma once
#include "Bolt-Graphics.h"

namespace Bolt
{

	class BLT_API Application
	{
	private:
		Timer* m_TickTimer;

	public:
		std::unique_ptr<Window> AppWindow;

	public:
		virtual ~Application();

		float Width() const;
		float Height() const;

		void Start();

		virtual void Init();
		virtual void Tick();
		virtual void Update();
		virtual void Render();

		friend class Engine;

	private:
		void CreateWindowPtr(const WindowCreateInfo& createInfo);

	};

}