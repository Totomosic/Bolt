#pragma once
#include "bltpch.h"

namespace DND
{

	class DndClient;

	void CreateServerScene(DndClient& client, Scene& serverScene, const ResourcePack& resources, Scene& gameScene);

}