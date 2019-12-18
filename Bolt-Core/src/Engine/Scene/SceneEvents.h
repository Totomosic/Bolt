#pragma once
#include "Core/Events/EventBus.h"
#include <any>

namespace Bolt
{

	struct Scene;

	class BLT_API SceneLoadedEvent
	{
	public:
		Scene* LoadedScene;
		std::any LoadData;
	};

	class BLT_API SceneUnloadedEvent
	{
	public:
		Scene* UnloadedScene;
	};

}