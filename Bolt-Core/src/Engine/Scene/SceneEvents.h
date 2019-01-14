#pragma once
#include "Bolt-Core.h"

namespace Bolt
{

	struct Scene;

	class BLT_API SceneLoadedEvent : public Event
	{
	public:
		Scene* LoadedScene;
	};

	class BLT_API SceneUnloadedEvent : public Event
	{
	public:
		Scene* UnloadedScene;
	};

}