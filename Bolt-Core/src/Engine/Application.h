#pragma once
#include "Bolt-Graphics.h"

namespace Bolt
{

	class BLT_API Application
	{
	private:
		id_t m_TickTimer;

	public:
		Window* PrimaryWindow;

	public:
		virtual ~Application();

		float Width() const;
		float Height() const;

		void Start(Window* engineWindow);
		void SetPrimaryWindow(Window* window);

		virtual void Init();
		virtual void Tick();
		virtual void Update();
		virtual void Render();

	};

}