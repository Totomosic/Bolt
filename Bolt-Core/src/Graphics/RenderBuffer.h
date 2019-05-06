#pragma once
#include "Core/Color.h"

namespace Bolt
{

	BLT_API enum class ColorBuffer
	{
		Color0 = GL_COLOR_ATTACHMENT0,
		Color1 = GL_COLOR_ATTACHMENT1,
		Color2 = GL_COLOR_ATTACHMENT2,
		Color3 = GL_COLOR_ATTACHMENT3,
		Color4 = GL_COLOR_ATTACHMENT4,
		Color5 = GL_COLOR_ATTACHMENT5,
		Depth = GL_DEPTH_ATTACHMENT
	};

	struct BLT_API RenderBuffer
	{
	public:
		id_t ID;
		int Width;
		int Height;
		int Samples;
		ColorBuffer Buffer;
	};

}