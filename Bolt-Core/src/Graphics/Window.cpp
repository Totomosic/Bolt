#include "Types.h"
#include "Window.h"
#include "Engine\User\Input.h"

namespace Bolt
{

	Window::Window(int width, int height, const blt::string& title, const WindowCreateInfo& info)
		: m_WindowHandle(nullptr), m_Framebuffer(), m_Title(title), m_CreateInfo(info),
		OnResize(Events::WINDOW_RESIZE), OnMoved(Events::WINDOW_MOVED), OnFocus(Events::WINDOW_FOCUSED), OnFocusLost(Events::WINDOW_LOST_FOCUS)
	{
		m_Framebuffer.GetViewport().Width = width;
		m_Framebuffer.GetViewport().Height = height;
		
		glfwWindowHint(GLFW_DECORATED, info.Decorated);
		glfwWindowHint(GLFW_RESIZABLE, info.Resizable);
		glfwWindowHint(GLFW_VISIBLE, info.Visible);
		glfwWindowHint(GLFW_FOCUSED, info.Focused);
		glfwWindowHint(GLFW_FLOATING, info.AlwaysOnTop);
		glfwWindowHint(GLFW_RED_BITS, info.RedBits);
		glfwWindowHint(GLFW_GREEN_BITS, info.GreenBits);
		glfwWindowHint(GLFW_BLUE_BITS, info.BlueBits);
		glfwWindowHint(GLFW_ALPHA_BITS, info.AlphaBits);
		glfwWindowHint(GLFW_DEPTH_BITS, info.DepthBits);
		glfwWindowHint(GLFW_SAMPLES, info.Samples);
		m_Framebuffer.m_Samples = info.Samples;

		m_WindowHandle = glfwCreateWindow(Width(), Height(), m_Title.c_str(), nullptr, nullptr);
		MakeCurrent();
		if (info.Maximised)
		{
			Maximize();
		}

		glfwSetCursorPosCallback(m_WindowHandle, Input::MousePositionCallback);
		glfwSetScrollCallback(m_WindowHandle, Input::MouseScrollCallback);
		glfwSetMouseButtonCallback(m_WindowHandle, Input::MouseButtonCallback);
		glfwSetKeyCallback(m_WindowHandle, Input::KeyboardKeyCallback);
		glfwSetFramebufferSizeCallback(m_WindowHandle, Input::WindowResizeCallback);
		glfwSetCharCallback(m_WindowHandle, Input::CharPressedCallback);
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_WindowHandle);
	}

	const Framebuffer& Window::GetFramebuffer() const
	{
		return m_Framebuffer;
	}

	int Window::Width() const
	{
		return m_Framebuffer.Width();
	}

	int Window::Height() const
	{
		return m_Framebuffer.Height();
	}

	Vector2i Window::Size() const
	{
		return Vector2i(Width(), Height());
	}

	const blt::string& Window::Title() const
	{
		return m_Title;
	}

	const Color& Window::ClearColor() const
	{
		return m_Framebuffer.ClearColor();
	}

	Color& Window::ClearColor()
	{
		return m_Framebuffer.ClearColor();
	}

	GLFWwindow* Window::WindowHandle() const
	{
		return m_WindowHandle;
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_WindowHandle);
	}

	void Window::Close() const
	{
 		glfwSetWindowShouldClose(m_WindowHandle, true);
	}

	void Window::Clear(ClearBuffer buffer) const
	{
		m_Framebuffer.Clear(buffer);
	}

	void Window::SwapBuffers() const
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	void Window::MakeCurrent() const
	{
		glfwMakeContextCurrent(m_WindowHandle);
	}

	void Window::EnableVSync() const
	{
		glfwSwapInterval(1);
	}

	void Window::DisableVSync() const
	{
		glfwSwapInterval(0);
	}

	void Window::CentreOnMonitor(const Monitor& monitor)
	{
		SetPosition(monitor.Width / 2 - Width() / 2, monitor.Height / 2 - Height() / 2);
	}

	void Window::Minimize() const
	{
		glfwIconifyWindow(m_WindowHandle);
	}

	void Window::Restore() const
	{
		glfwRestoreWindow(m_WindowHandle);
	}

	void Window::Maximize()
	{
		Monitor main = Monitor::Primary();
		m_Framebuffer.GetViewport().Width = main.Width;
		m_Framebuffer.GetViewport().Height = (m_CreateInfo.Decorated) ? main.Height - 63 : main.Height;
		glfwMaximizeWindow(m_WindowHandle);
	}

	void Window::Hide() const
	{
		glfwHideWindow(m_WindowHandle);
	}

	void Window::Show() const
	{
		glfwShowWindow(m_WindowHandle);
	}

	void Window::SetSize(const Vector2i& size)
	{
		SetSize(size.x, size.y);
	}

	void Window::SetSize(int width, int height)
	{
		std::unique_ptr<WindowResizeEvent> args = std::make_unique<WindowResizeEvent>();
		args->OldWidth = Width();
		args->OldHeight = Height();
		args->NewWidth = width;
		args->NewHeight = height;
		m_Framebuffer.GetViewport().Width = width;
		m_Framebuffer.GetViewport().Height = height;
		glfwSetWindowSize(m_WindowHandle, Width(), Height());
		OnResize.Post(std::move(args));
	}

	void Window::SetWidth(int width)
	{
		SetSize(width, Height());
	}

	void Window::SetHeight(int height)
	{
		SetSize(Width(), height);
	}

	void Window::SetTitle(const blt::string& text)
	{
		m_Title = text;
		glfwSetWindowTitle(m_WindowHandle, m_Title.c_str());
	}

	void Window::SetClearColor(const Color& clearColor)
	{
		m_Framebuffer.ClearColor() = clearColor;
	}

	void Window::SetPosition(const Vector2i& position)
	{
		SetPosition(position.x, position.y);
	}

	void Window::SetPosition(int x, int y)
	{
		glfwSetWindowPos(m_WindowHandle, x, y);
	}

}