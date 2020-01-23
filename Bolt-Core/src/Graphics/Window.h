#pragma once
#include "Framebuffer.h"
#include "Structs/Monitor.h"
#include "Structs/WindowCreateInfo.h"
#include "WindowEvents.h"

namespace Bolt
{

	class AppContext;

	struct BLT_API WindowEvents
	{
	public:
		EventEmitter<WindowResizeEvent> m_OnResize;
		EventEmitter<WindowMovedEvent> m_OnMoved;
		EventEmitter<WindowFocusedEvent> m_OnFocused;
		EventEmitter<WindowClosedEvent> m_OnClosed;
	};

	struct BLT_API WindowData
	{
	public:
		AppContext* m_Context;
		GLFWwindow* m_WindowHandle;
		Framebuffer m_Framebuffer;
		std::string m_Title;
		bool m_IsDecorated;
		bool m_IsFocused;
	};

	class BLT_API Window
	{
	private:
		static bool s_IsGLADInitialized;

	private:
		EventBus m_EventBus;
		WindowEvents m_Events;
		WindowData m_Data;

	public:
		Window(AppContext* context, const WindowCreateInfo& info = WindowCreateInfo());
		Window(const Window& other) = delete;
		Window(Window&& other) = delete;
		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other) = delete;
		~Window();

		inline EventBus& Events() { return m_EventBus; }
		inline EventEmitter<WindowResizeEvent>& OnResize() { return m_Events.m_OnResize; }
		inline EventEmitter<WindowMovedEvent>& OnMoved() { return m_Events.m_OnMoved; }
		inline EventEmitter<WindowFocusedEvent>& OnFocused() { return m_Events.m_OnFocused; }
		inline EventEmitter<WindowClosedEvent>& OnClose() { return m_Events.m_OnClosed; }

		AppContext& GetContext() const;
		const Framebuffer& GetFramebuffer() const;
		int Width() const;
		int Height() const;
		float Aspect() const;
		Vector2i Size() const;
		const std::string& Title() const;
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
		void SetTitle(const std::string& title);
		void SetClearColor(const Color& clearColor);
		void SetPosition(const Vector2i& position);
		void SetPosition(int x, int y);
		void SetIsFocused(bool isFocused);

		void MakeCurrent() const;

	};

}