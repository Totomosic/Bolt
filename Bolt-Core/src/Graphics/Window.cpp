#include "Types.h"

#include "Window.h"
#include "Engine\User\Input.h"

namespace Bolt
{

	Window::Window(const WindowCreateInfo& info)
		: m_Data{ nullptr, Framebuffer(), info.Title, info.Decorated }, OnResize(Events::WINDOW_RESIZE), OnMoved(Events::WINDOW_MOVED), OnFocus(Events::WINDOW_FOCUSED), OnFocusLost(Events::WINDOW_LOST_FOCUS)
	{
		m_Data.m_Framebuffer.GetViewport().Width = info.Width;
		m_Data.m_Framebuffer.GetViewport().Height = info.Height;
		m_Data.m_Framebuffer.ClearColor() = info.ClearColor;
		
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
		m_Data.m_Framebuffer.m_Samples = info.Samples;

		m_Data.m_WindowHandle = glfwCreateWindow(Width(), Height(), Title().c_str(), nullptr, nullptr);
		MakeCurrent();
		if (info.Maximised)
		{
			Maximize();
		}

		glfwSetCursorPosCallback(WindowHandle(), Input::MousePositionCallback);
		glfwSetScrollCallback(WindowHandle(), Input::MouseScrollCallback);
		glfwSetMouseButtonCallback(WindowHandle(), Input::MouseButtonCallback);
		glfwSetKeyCallback(WindowHandle(), Input::KeyboardKeyCallback);
		glfwSetFramebufferSizeCallback(WindowHandle(), Input::WindowResizeCallback);
		glfwSetCharCallback(WindowHandle(), Input::CharPressedCallback);
	}

	Window::~Window()
	{
		glfwDestroyWindow(WindowHandle());
	}

	const Framebuffer& Window::GetFramebuffer() const
	{
		return m_Data.m_Framebuffer;
	}

	int Window::Width() const
	{
		return GetFramebuffer().Width();
	}

	int Window::Height() const
	{
		return GetFramebuffer().Height();
	}

	float Window::Aspect() const
	{
		return GetFramebuffer().Aspect();
	}

	Vector2i Window::Size() const
	{
		return Vector2i(Width(), Height());
	}

	const blt::string& Window::Title() const
	{
		return m_Data.m_Title;
	}

	const Color& Window::ClearColor() const
	{
		return GetFramebuffer().ClearColor();
	}

	Color& Window::ClearColor()
	{
		return m_Data.m_Framebuffer.ClearColor();
	}

	GLFWwindow* Window::WindowHandle() const
	{
		return m_Data.m_WindowHandle;
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(WindowHandle());
	}

	void Window::Close() const
	{
 		glfwSetWindowShouldClose(WindowHandle(), true);
	}

	void Window::Clear(ClearBuffer buffer) const
	{
		GetFramebuffer().Clear(buffer);
	}

	void Window::SwapBuffers() const
	{
		glfwSwapBuffers(WindowHandle());
	}

	void Window::MakeCurrent() const
	{
		glfwMakeContextCurrent(WindowHandle());
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
		glfwIconifyWindow(WindowHandle());
	}

	void Window::Restore() const
	{
		glfwRestoreWindow(WindowHandle());
	}

	void Window::Maximize()
	{
		Monitor main = Monitor::Primary();
		m_Data.m_Framebuffer.GetViewport().Width = main.Width;
		m_Data.m_Framebuffer.GetViewport().Height = (m_Data.m_IsDecorated) ? main.Height - 63 : main.Height;
		glfwMaximizeWindow(WindowHandle());
	}

	void Window::Hide() const
	{
		glfwHideWindow(WindowHandle());
	}

	void Window::Show() const
	{
		glfwShowWindow(WindowHandle());
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
		m_Data.m_Framebuffer.GetViewport().Width = width;
		m_Data.m_Framebuffer.GetViewport().Height = height;
		glfwSetWindowSize(WindowHandle(), Width(), Height());
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
		m_Data.m_Title = text;
		glfwSetWindowTitle(WindowHandle(), Title().c_str());
	}

	void Window::SetClearColor(const Color& clearColor)
	{
		m_Data.m_Framebuffer.ClearColor() = clearColor;
	}

	void Window::SetPosition(const Vector2i& position)
	{
		SetPosition(position.x, position.y);
	}

	void Window::SetPosition(int x, int y)
	{
		glfwSetWindowPos(WindowHandle(), x, y);
	}

}