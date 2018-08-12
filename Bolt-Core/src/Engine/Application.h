#pragma once
#include "Bolt-Graphics.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw_gl3.h"

namespace Bolt
{

	class BLT_API Application
	{
	private:
		Timer * m_TickTimer;

	public:
		Window* PrimaryWindow;

	public:
		virtual ~Application();

		void Start(Window* engineWindow);
		void SetPrimaryWindow(Window* window);

		virtual void Init();
		virtual void Tick();
		virtual void Update();
		virtual void ImGuiRender();
		virtual void Render();

	};

}