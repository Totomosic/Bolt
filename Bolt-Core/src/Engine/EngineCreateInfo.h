#pragma once
#include "Graphics/Structs/WindowCreateInfo.h"

namespace Bolt
{

	struct BLT_API EngineCreateInfo
	{
	public:
		bool UseGraphics = true;
		bool UseSockets = true;
		WindowCreateInfo WindowInfo;
	};

}