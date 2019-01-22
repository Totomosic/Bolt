#pragma once
#include "Types.h"

namespace Bolt
{

	struct BLT_API EngineCreateInfo
	{
	public:
		bool UseGraphics = true;
		bool UseSockets = true;
	};

}