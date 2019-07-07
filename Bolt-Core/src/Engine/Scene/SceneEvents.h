#pragma once
#include "Core/Events/EventBus.h"

namespace Bolt
{

	struct Scene;

	class BLT_API SceneLoadedEvent
	{
	public:
		Scene* LoadedScene;
		std::unique_ptr<EventContainer> LoadData;
	};

	class BLT_API SceneUnloadedEvent
	{
	public:
		Scene* UnloadedScene;
	};

}