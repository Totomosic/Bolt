#include "bltpch.h"

#include "Initializer.h"
#include "../User/Input.h"
#include "../Renderer/Graphics.h"
#include "Core/Utils/Filesystem/Filesystem.h"

#include "Core/Profiling/Profiling.h"

namespace Bolt
{

	void Initializer::PreOpenGL(const EngineCreateInfo& createInfo)
	{
		BLT_PROFILE_FUNCTION();
		Log::Initialize();
		BLT_CORE_INFO("Logger Initialized");
		Filesystem::Initialize();
		BLT_CORE_INFO("Filesystem Initialized");
		if (createInfo.UseGraphics)
		{
			BLT_PROFILE_SCOPE("glfwInit()");
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
			BLT_PROFILE_SCOPE("InitSockets()");
#ifdef BLT_PLATFORM_WINDOWS
			WSADATA data;
			if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
			{
				BLT_ASSERT(false, "Failed to Initialize WinSock2");
			}
			BLT_CORE_INFO("Socket Initialized");
#endif
		}
		else
		{
			BLT_CORE_WARN("Skipped Initializing Socket");
		}
	}

}