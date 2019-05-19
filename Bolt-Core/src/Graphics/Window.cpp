#include "bltpch.h"

#include "Window.h"
#include "Engine/User/Input.h"
#include "Engine/AppContext.h"

namespace Bolt
{

	bool Window::s_IsGLADInitialized = false;

	Window::Window(AppContext* context, const WindowCreateInfo& info)
		: m_Data{ {}, {}, {}, {}, {}, context, nullptr, Framebuffer(), info.Title, info.Decorated, true }
	{
		m_Data.m_Framebuffer.GetViewport().Width = info.Width;
		m_Data.m_Framebuffer.GetViewport().Height = info.Height;
		m_Data.m_Framebuffer.ClearColor() = info.ClearColor;
		
		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, info.TransparentFramebuffer);
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
		if (!s_IsGLADInitialized)
		{
			int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			if (result == 0)
			{
				BLT_ASSERT(false, "Failed to Initialize Glad");
			}
			BLT_CORE_INFO("Glad Initialized");
			BLT_CORE_INFO("OpenGL Info:");
			BLT_CORE_INFO("Graphics Vendor: {0}", glGetString(GL_VENDOR));
			BLT_CORE_INFO("Graphics Card: {0}", glGetString(GL_RENDERER));
			BLT_CORE_INFO("OpenGL Version: {0}", glGetString(GL_VERSION));
			s_IsGLADInitialized = true;
		}
		if (info.Maximised)
		{
			Maximize();
		}
		glfwSetWindowUserPointer((GLFWwindow*)GetNativeWindow(), this);

		glfwSetCursorPosCallback((GLFWwindow*)GetNativeWindow(), [](GLFWwindow * windowHandle, double x, double y)
			{
				Window& window = *(Window*)glfwGetWindowUserPointer(windowHandle);
				window.GetContext().GetRenderContext().GetInput().MousePositionCallback(x, y);
			});
		glfwSetScrollCallback((GLFWwindow*)GetNativeWindow(), [](GLFWwindow * windowHandle, double x, double y)
			{
				Window& window = *(Window*)glfwGetWindowUserPointer(windowHandle);
				window.GetContext().GetRenderContext().GetInput().MouseScrollCallback(x, y);
			});
		glfwSetMouseButtonCallback((GLFWwindow*)GetNativeWindow(), [](GLFWwindow * windowHandle, int button, int action, int mods)
			{
				Window& window = *(Window*)glfwGetWindowUserPointer(windowHandle);
				window.GetContext().GetRenderContext().GetInput().MouseButtonCallback(button, action, mods);
			});
		glfwSetKeyCallback((GLFWwindow*)GetNativeWindow(), [](GLFWwindow * windowHandle, int key, int scancode, int action, int mods)
			{
				Window& window = *(Window*)glfwGetWindowUserPointer(windowHandle);
				window.GetContext().GetRenderContext().GetInput().KeyboardKeyCallback(key, scancode, action, mods);
			});
		glfwSetCharCallback((GLFWwindow*)GetNativeWindow(), [](GLFWwindow * windowHandle, uint32_t character)
			{
				Window& window = *(Window*)glfwGetWindowUserPointer(windowHandle);
				window.GetContext().GetRenderContext().GetInput().CharPressedCallback(character);
			});
		glfwSetFramebufferSizeCallback((GLFWwindow*)GetNativeWindow(), [](GLFWwindow * windowHandle, int width, int height)
			{
				Window& window = *(Window*)glfwGetWindowUserPointer(windowHandle);
				window.SetSize(width, height);
			});
		glfwSetWindowCloseCallback((GLFWwindow*)GetNativeWindow(), [](GLFWwindow * windowHandle)
			{
				Window& window = *(Window*)glfwGetWindowUserPointer(windowHandle);
				window.OnClose().Post(WindowClosedEvent());
			});
		glfwSetWindowFocusCallback((GLFWwindow*)GetNativeWindow(), [](GLFWwindow* windowHandle, int focus)
			{
				Window& window = *(Window*)glfwGetWindowUserPointer(windowHandle);
				window.SetIsFocused(focus == GLFW_TRUE);
			});
	}

	Window::~Window()
	{
		glfwDestroyWindow((GLFWwindow*)GetNativeWindow());
	}

	AppContext& Window::GetContext() const
	{
		return *m_Data.m_Context;
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

	bool Window::IsFocused() const
	{
		return m_Data.m_IsFocused;
	}

	void* Window::GetNativeWindow() const
	{
		return m_Data.m_WindowHandle;
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose((GLFWwindow*)GetNativeWindow());
	}

	void Window::Close() const
	{
 		glfwSetWindowShouldClose((GLFWwindow*)GetNativeWindow(), true);
	}

	void Window::Clear(ClearBuffer buffer) const
	{
		GetFramebuffer().Clear(buffer);
	}

	void Window::SwapBuffers() const
	{
		glfwSwapBuffers((GLFWwindow*)GetNativeWindow());
	}

	void Window::MakeCurrent() const
	{
		glfwMakeContextCurrent((GLFWwindow*)GetNativeWindow());
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
		glfwIconifyWindow((GLFWwindow*)GetNativeWindow());
	}

	void Window::Restore() const
	{
		glfwRestoreWindow((GLFWwindow*)GetNativeWindow());
	}

	void Window::Maximize()
	{
		Monitor main = Monitor::Primary();
		m_Data.m_Framebuffer.GetViewport().Width = main.Width;
		m_Data.m_Framebuffer.GetViewport().Height = (m_Data.m_IsDecorated) ? main.Height - 63 : main.Height;
		glfwMaximizeWindow((GLFWwindow*)GetNativeWindow());
	}

	void Window::Hide() const
	{
		glfwHideWindow((GLFWwindow*)GetNativeWindow());
	}

	void Window::Show() const
	{
		glfwShowWindow((GLFWwindow*)GetNativeWindow());
	}

	void Window::SetSize(const Vector2i& size)
	{
		SetSize(size.x, size.y);
	}

	void Window::SetSize(int width, int height)
	{
		WindowResizeEvent args;
		args.OldWidth = Width();
		args.OldHeight = Height();
		args.NewWidth = width;
		args.NewHeight = height;
		m_Data.m_Framebuffer.GetViewport().Width = width;
		m_Data.m_Framebuffer.GetViewport().Height = height;
		glfwSetWindowSize((GLFWwindow*)GetNativeWindow(), width, height);
		OnResize().Post(std::move(args));
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
		glfwSetWindowTitle((GLFWwindow*)GetNativeWindow(), Title().c_str());
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
		glfwSetWindowPos((GLFWwindow*)GetNativeWindow(), x, y);
	}

	void Window::SetIsFocused(bool isFocused)
	{
		m_Data.m_IsFocused = isFocused;
	}

}