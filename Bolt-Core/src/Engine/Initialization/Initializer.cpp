#include "Types.h"

#include "Initializer.h"
#include "..\User\Input.h"
#include "..\Renderer\Graphics.h"
#include "..\Scene\ObjectFactory.h"
#include "..\Commands\CmdDebugger.h"

namespace Bolt
{

	void Initializer::PreOpenGL(const EngineCreateInfo& createInfo)
	{
		Log::Initialize();
		BLT_CORE_INFO("Logger Initialized");
		EventManager::Initialize();
		BLT_CORE_INFO("EventManager Initialized");
		Filesystem::Initialize();
		BLT_CORE_INFO("Filesystem Intialized");
		if (createInfo.UseGraphics)
		{
			int result = glfwInit();
			BLT_ASSERT(result != GL_NO_ERROR, "GLFW failed to initialize");
			BLT_CORE_INFO("GLFW Initialized");
		}
		else
		{
			BLT_CORE_WARN("Skipped Initializing GLFW");
		}
		Random::Initialize();
		BLT_CORE_INFO("Random Initialized");
		if (createInfo.UseSockets)
		{
			WSADATA data;
			if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
			{
				BLT_ASSERT(false, "Failed to initialize WinSock2");
			}
			BLT_CORE_INFO("Socket Initialized");
		}
		else
		{
			BLT_CORE_WARN("Skipped Initializing Socket");
		}
	}

	void Initializer::PostOpenGL(const EngineCreateInfo& createInfo, Window* window)
	{
		if (createInfo.UseGraphics)
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
		}
		else
		{
			BLT_CORE_WARN("Skipped Initializing Glad");
			BLT_CORE_WARN("Skipped Initializing Input");
			BLT_CORE_WARN("Skipped Initializing Shader");
			BLT_CORE_WARN("Skipped Initializing Graphics");
			BLT_CORE_WARN("Skipped Initializing ObjectFactory");
		}
		CmdDebugger::Initialize();
		BLT_CORE_INFO("Command line Debugger Initialized");
	}

}