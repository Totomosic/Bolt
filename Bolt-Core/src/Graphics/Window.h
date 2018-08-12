#pragma once
#include "Framebuffer.h"
#include "Structs\Monitor.h"
#include "Structs\WindowCreateInfo.h"

namespace Bolt
{

	class BLT_API Window
	{
	private:
		GLFWwindow* m_WindowHandle;
		Framebuffer m_Framebuffer;
		blt::string m_Title;
		WindowCreateInfo m_CreateInfo;

	public:
		Window(int width, int height, const blt::string& title, const WindowCreateInfo& info = WindowCreateInfo());
		Window(const Window& other) = delete;
		Window(Window&& other) = delete;
		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other) = delete;
		~Window();

		const Framebuffer& GetFramebuffer() const;
		int Width() const;
		int Height() const;
		Vector2i Size() const;
		const blt::string& Title() const;
		const Color& ClearColor() const;
		Color& ClearColor();
		GLFWwindow* WindowHandle() const;

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

		friend class Application;
		friend class WindowManager;

	private:
		void MakeCurrent() const;

	};

}