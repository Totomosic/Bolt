#pragma once
#include "GameObject.h"

namespace Bolt
{

	struct BLT_API InstantiatedEvent
	{
	public:
		GameObject* Object;
	};

	struct BLT_API DestroyedEvent
	{
	public:
		GameObject* Object;
		float TimeToDelete;
	};

}