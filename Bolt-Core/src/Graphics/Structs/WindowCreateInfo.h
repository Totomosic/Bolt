#pragma once
#include "Bolt-Core.h"

namespace Bolt
{

	struct BLT_API WindowCreateInfo
	{
	public:
		bool Fullscreen = false;
		bool Decorated = true;
		bool Maximised = false;
		bool Resizable = true;
		bool Visible = true;
		bool Focused = true;
		bool AlwaysOnTop = false;

		int RedBits = 8;
		int GreenBits = 8;
		int BlueBits = 8;
		int AlphaBits = 8;
		int DepthBits = 32;

		int Samples = 4;

	};

}