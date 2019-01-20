#pragma once
#include "Bolt-Graphics.h"

namespace Bolt
{

	class BLT_API Destructor
	{
	public:
		Destructor() = delete;

		static void Run(std::unique_ptr<Window>&& window);

	};

}