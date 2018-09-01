#pragma once
#include "Bolt-Graphics.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw_gl3.h"

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
		virtual void ImGuiRender();
		virtual void Render();

	};

}