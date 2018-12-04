#pragma once
#include "Bolt-Core.h"

namespace Bolt
{

	struct BLT_API WindowResizeEvent : public Event
	{
	public:
		int OldWidth;
		int OldHeight;
		int NewWidth;
		int NewHeight;
	};

	struct BLT_API WindowMovedEvent : public Event
	{
	public:
		int x;
		int y;
		int relX;
		int relY;
	};

	struct BLT_API WindowFocusedEvent : public Event
	{
	public:

	};

}