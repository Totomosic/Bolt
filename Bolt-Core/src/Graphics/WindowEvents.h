#pragma once
#include "Core/Events/EventBus.h"

namespace Bolt
{

	struct BLT_API WindowResizeEvent
	{
	public:
		int OldWidth;
		int OldHeight;
		int NewWidth;
		int NewHeight;
	};

	struct BLT_API WindowMovedEvent
	{
	public:
		int x;
		int y;
		int relX;
		int relY;
	};

	struct BLT_API WindowFocusedEvent
	{

	};

	struct BLT_API WindowClosedEvent
	{

	};

}