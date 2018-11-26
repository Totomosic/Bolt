#include "Initializer.h"
#include "..\User\Input.h"
#include "..\Renderer\Graphics.h"
#include "..\Scene\ObjectFactory.h"

namespace Bolt
{

	bool Initializer::Run(Window* window)
	{
		int result = glewInit();
		if (result != GLEW_OK)
		{
			return false;
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