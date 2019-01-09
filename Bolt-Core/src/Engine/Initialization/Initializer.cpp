#include "Types.h"

#include "Initializer.h"
#include "..\User\Input.h"
#include "..\Renderer\Graphics.h"
#include "..\Scene\ObjectFactory.h"

namespace Bolt
{

	bool Initializer::Run(Window* window)
	{
		int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (result == 0)
		{
			BLT_ASSERT(false, "Failed to initialize Glad");
		}
		Log::Initialize();
		BLT_CORE_INFO("Logger Initialized");
		BLT_CORE_INFO("GLEW Initialized");
		Input::Initialize(window);
		BLT_CORE_INFO("Input Initialized");
		Shader::Initialize();
		BLT_CORE_INFO("Shader Initialized");
		Graphics::Initialize(window);
		BLT_CORE_INFO("Graphics Initialized");
		ObjectFactory::Initialize();
		BLT_CORE_INFO("ObjectFactory Initialized");
		return true;
	}

}