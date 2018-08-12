#include "Initializor.h"
#include "..\User\Input.h"
#include "..\Renderer\Graphics.h"

namespace Bolt
{

	bool Initializor::Run(Window* window)
	{
		int result = glewInit();
		if (result != GLEW_OK)
		{
			return false;
		}
		Input::Initialize(window);
		Shader::Initialize();
		Graphics::Initialize();
		return true;
	}

}