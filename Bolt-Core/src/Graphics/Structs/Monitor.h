#pragma once
#include "bltpch.h"

namespace Bolt
{

	struct BLT_API Monitor
	{
	public:
		GLFWmonitor* Handle = nullptr;
		const GLFWvidmode* VideoMode = nullptr;
		int Width;
		int Height;
		std::string Name;

		int RedBits;
		int GreenBits;
		int BlueBits;
		int AlphaBits;

	public:
		static Monitor Primary();

	};

}