#pragma once
#include "bltpch.h"
#include "../Networking/NetworkPackets.h"
#include "../GameManager.h"

namespace DND
{

	class DndClient;

	void CreateGameScene(DndClient& client, Scene& gameScene, const ResourcePack& resources);
	void CreateTilemap(const ResourcePack& resources);
	void CreateSceneFromWelcome(const PlayerCharacterInfo& playerInfo);

}