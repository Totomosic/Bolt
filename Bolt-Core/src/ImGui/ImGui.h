#pragma once
#include "bltpch.h"

namespace Bolt
{

	class Window;

	bool BLT_API InitImGui(Window& window);
	bool BLT_API ShutdownImGui();

}