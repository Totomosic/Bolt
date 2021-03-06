#pragma once
#include "BoltLib/Color.h"

namespace Bolt
{

	struct BLT_API WindowCreateInfo
	{
	public:
		int Width = 1280;
		int Height = 720;
		std::string Title = "Bolt";
		Color ClearColor = Color::Black;

		bool Fullscreen = false;
		bool Decorated = true;
		bool Maximised = false;
		bool Resizable = true;
		bool Visible = true;
		bool Focused = true;
		bool AlwaysOnTop = false;
		bool TransparentFramebuffer = false;

		int RedBits = 8;
		int GreenBits = 8;
		int BlueBits = 8;
		int AlphaBits = 8;
		int DepthBits = 32;

		int Samples = 4;

	};

}