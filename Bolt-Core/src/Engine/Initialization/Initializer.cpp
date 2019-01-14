#include "Types.h"

#include "Initializer.h"
#include "..\User\Input.h"
#include "..\Renderer\Graphics.h"
#include "..\Scene\ObjectFactory.h"
#include "..\Commands\CmdDebugger.h"

namespace Bolt
{

	void Initializer::PreOpenGL()
	{
		Log::Initialize();
		BLT_CORE_INFO("Logger Initialized");
		Filesystem::Initialize();
		BLT_CORE_INFO("Filesystem Intialized");
		int result = glfwInit();
		BLT_ASSERT(result != GL_NO_ERROR, "GLFW failed to initialize");
		BLT_CORE_INFO("GLFW Initialized");
		Random::Initialize();
		BLT_CORE_INFO("Random Initialized");
		WSADATA data;
		if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
		{
			BLT_ASSERT(false, "Failed to initialize WinSock2");
		}
		BLT_CORE_INFO("Socket Initialized");
	}

	void Initializer::PostOpenGL(Window* window)
	{
		int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (result == 0)
		{
			BLT_ASSERT(false, "Failed to initialize Glad");
		}
		BLT_CORE_INFO("Glad Initialized");
		Input::Initialize(window);
		BLT_CORE_INFO("Input Initialized");
		Shader::Initialize();
		BLT_CORE_INFO("Shader Initialized");
		Graphics::Initialize(window);
		BLT_CORE_INFO("Graphics Initialized");
		ObjectFactory::Initialize();
		BLT_CORE_INFO("ObjectFactory Initialized");
		CmdDebugger::Initialize();
		BLT_CORE_INFO("Command line Debugger Initialized");
	}

}