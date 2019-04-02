#pragma once
#include "BoltEngine.h"

using namespace Bolt;

namespace Aimbooster
{

	class TargetHitEvent : public Event
	{
	public:
		BLT_EVENT_ID_DEF(EventManager::USER_EVENT_ID + 1);
	};

	class TargetFailedEvent : public Event
	{
	public:
		BLT_EVENT_ID_DEF(EventManager::USER_EVENT_ID + 2);
		Vector3f Position;
	};

}