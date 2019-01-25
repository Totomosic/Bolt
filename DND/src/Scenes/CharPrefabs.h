#pragma once
#include "bltpch.h"
#include "../PrefabList.h"
#include "../AnimationList.h"

namespace DND
{

	void CreateCharacterPrefabs(PrefabList& prefabs, AnimationList& animations, ObjectFactory& factory, const ResourcePack& resources);

}