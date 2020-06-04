#pragma once
#include "Logging.h"

namespace Bolt::Assets
{

	class BLT_API Module
	{
	public:
		virtual ~Module() = default;
		virtual int Run(int argc, const char** argv) = 0;
	};

}