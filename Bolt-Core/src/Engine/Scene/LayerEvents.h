#pragma once
#include "Bolt-Core.h"

namespace Bolt
{

	struct BLT_API InstantiatedEvent : public Event
	{
	public:
		GameObject* Object;
	};

	struct BLT_API DestroyedEvent : public Event
	{
	public:
		GameObject* Object;
		float TimeToDelete;
	};

}