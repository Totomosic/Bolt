#pragma once
#include "Map/TilemapManager.h"

namespace DND
{

	struct PrefabList
	{
	public:
		id_t BlueWizardCharacter;

	};

	class GlobalState
	{
	public:
		static PrefabList Prefabs;
		static ObjectFactory Factory;
		static TilemapManager MapManager;

	};

}