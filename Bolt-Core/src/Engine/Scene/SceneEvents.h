#pragma once
#include "Bolt-Core.h"

namespace Bolt
{

	struct Scene;

	class BLT_API SceneLoadedEvent : public Event
	{
	public:
		Scene* LoadedScene;
		std::unique_ptr<Event> LoadData;

		BLT_EVENT_ID_DEF(Events::SCENE_LOADED);
	};

	class BLT_API SceneUnloadedEvent : public Event
	{
	public:
		Scene* UnloadedScene;

		BLT_EVENT_ID_DEF(Events::SCENE_UNLOADED);
	};

}