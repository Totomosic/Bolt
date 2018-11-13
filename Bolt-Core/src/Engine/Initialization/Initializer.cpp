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
		Input::Initialize(window);
		Shader::Initialize();
		Graphics::Initialize(window);
		ObjectFactory::Initialize();
		return true;
	}

}