#include "bltpch.h"

#include "Initializer.h"
#include "../User/Input.h"
#include "../Renderer/Graphics.h"
#include "../Scene/ObjectFactory.h"

namespace Bolt
{

	void Initializer::PreOpenGL(const EngineCreateInfo& createInfo)
	{
		Log::Initialize();
		BLT_CORE_INFO("Logger Initialized");
		Filesystem::Initialize();
		BLT_CORE_INFO("Filesystem Initialized");
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
				BLT_ASSERT(false, "Failed to Initialize WinSock2");
			}
			BLT_CORE_INFO("Socket Initialized");
		}
		else
		{
			BLT_CORE_WARN("Skipped Initializing Socket");
		}
	}

}