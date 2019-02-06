#pragma once
#include "bltpch.h"
#include "../Players/Characters/Entities/EntityNetworkData.h"

namespace DND
{

	class StartGameEvent : public Event
	{
	public:
		bool IsHosting;
		EntityNetworkData Character;
	};

	Scene& CreateServerScene(const ResourcePack& resources);

}