#include "bltpch.h"
#include "GlobalState.h"

namespace DND
{

	PrefabList GlobalState::Prefabs = PrefabList();
	ObjectFactory GlobalState::Factory = ObjectFactory();
	TilemapManager GlobalState::MapManager = TilemapManager(50, 50);

}