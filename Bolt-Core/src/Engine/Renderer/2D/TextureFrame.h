#pragma once
#include "bltpch.h"

namespace Bolt
{

	// Represents texture coordinates for a rectangular texture frame
	struct BLT_API TextureFrame
	{
	public:
		float x = 0.0f;
		float y = 0.0f;
		float w = 1.0f;
		float h = 1.0f;
	};

}