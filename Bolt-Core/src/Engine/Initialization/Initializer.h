#pragma once
#include "Bolt-Graphics.h"

namespace Bolt
{

	class BLT_API Initializer
	{
	public:
		Initializer() = delete;

		static bool Run(Window* window);

	};

}