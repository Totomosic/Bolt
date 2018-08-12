#pragma once
#include "Bolt-Core.h"

namespace Bolt
{

	struct BLT_API Viewport
	{
	public:
		int X;
		int Y;
		int Width;
		int Height;

	public:
		inline void Bind() const
		{
			GL_CALL(glViewport(X, Y, Width, Height));
		}

	};

}