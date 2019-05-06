#pragma once
#include "Core/Events/EventManager.h"

namespace Bolt
{

	struct BLT_API WindowResizeEvent : public Event
	{
	public:
		int OldWidth;
		int OldHeight;
		int NewWidth;
		int NewHeight;

		BLT_EVENT_ID_DEF(Events::WINDOW_RESIZE);
	};

	struct BLT_API WindowMovedEvent : public Event
	{
	public:
		int x;
		int y;
		int relX;
		int relY;

		BLT_EVENT_ID_DEF(Events::WINDOW_MOVED);
	};

	struct BLT_API WindowFocusedEvent : public Event
	{
	public:
		BLT_EVENT_ID_DEF(Events::WINDOW_FOCUSED);
	};

	struct BLT_API WindowClosedEvent : public Event
	{
	public:
		BLT_EVENT_ID_DEF(Events::WINDOW_CLOSED);
	};

}