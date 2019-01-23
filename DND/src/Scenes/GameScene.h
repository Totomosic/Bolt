#pragma once
#include "bltpch.h"
#include "../GameManager.h"

namespace DND
{

	class DndClient;

	void CreateGameScene(DndClient& client, Scene& gameScene, const ResourcePack& resources);
	void CreateTilemap(const ResourcePack& resources);
	void CreateSceneFromWelcome(const WelcomePacket& packet, const PlayerCharacterInfo& playerInfo);

}