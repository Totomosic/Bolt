#pragma once
#include "Framebuffer.h"
#include "Structs\Monitor.h"
#include "Structs\WindowCreateInfo.h"
#include "WindowEvents.h"

namespace Bolt
{

	class AppContext;

	struct BLT_API WindowData
	{
	public:
		EventDispatcher<WindowResizeEvent> m_OnResize;
		EventDispatcher<WindowMovedEvent> m_OnMoved;
		EventDispatcher<WindowFocusedEvent> m_OnFocus;
		EventDispatcher<WindowFocusedEvent> m_OnFocusLost;
		EventDispatcher<WindowClosedEvent> m_OnClose;

		AppContext* m_Context;
		GLFWwindow* m_WindowHandle;
		Framebuffer m_Framebuffer;
		blt::string m_Title;
		bool m_IsDecorated;
		bool m_IsFocused;
	};

	class BLT_API Window
	{
	private:
		static bool s_IsGLADInitialized;

	private:
		WindowData m_Data;

	public:
		Window(AppContext* context, const WindowCreateInfo& info = WindowCreateInfo());
		Window(const Window& other) = delete;
		Window(Window&& other) = delete;
		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other) = delete;
		~Window();

		inline EventDispatcher<WindowResizeEvent>& OnResize() { return m_Data.m_OnResize; }
		inline EventDispatcher<WindowMovedEvent>& OnMoved() { return m_Data.m_OnMoved; }
		inline EventDispatcher<WindowFocusedEvent>& OnFocus() { return m_Data.m_OnFocus; }
		inline EventDispatcher<WindowFocusedEvent>& OnFocusLost() { return m_Data.m_OnFocusLost; }
		inline EventDispatcher<WindowClosedEvent>& OnClose() { return m_Data.m_OnClose; }

		AppContext& GetContext() const;
		const Framebuffer& GetFramebuffer() const;
		int Width() const;
		int Height() const;
		float Aspect() const;
		Vector2i Size() const;
		const blt::string& Title() const;
		const Color& ClearColor() const;
		Color& ClearColor();
		bool IsFocused() const;
		void* GetNativeWindow() const;

		bool ShouldClose() const;
		void Close() const;
		void Clear(ClearBuffer clearBuffer = ClearBuffer::Color | ClearBuffer::Depth) const;
		void SwapBuffers() const;

		void EnableVSync() const;
		void DisableVSync() const;
		void CentreOnMonitor(const Monitor& monitor);
		void Minimize() const;
		void Restore() const;
		void Maximize();
		void Hide() const;
		void Show() const;

		void SetSize(const Vector2i& size);
		void SetSize(int width, int height);
		void SetWidth(int width);
		void SetHeight(int height);
		void SetTitle(const blt::string& title);
		void SetClearColor(const Color& clearColor);
		void SetPosition(const Vector2i& position);
		void SetPosition(int x, int y);
		void SetIsFocused(bool isFocused);

		void MakeCurrent() const;

	};

}