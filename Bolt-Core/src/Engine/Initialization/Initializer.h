#pragma once
#include "Bolt-Graphics.h"

namespace Bolt
{

	class BLT_API Initializer
	{
	public:
		Initializer() = delete;

		static void PreOpenGL();
		static void PostOpenGL(Window* window);

	};

}