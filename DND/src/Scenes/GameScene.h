#pragma once
#include "bltpch.h"
#include "../Map/TilemapManager.h"

namespace DND
{

	struct StartGameHandler
	{
	public:
		TilemapManager MapManager;
		ObjectFactory Factory;
		Layer& EntityLayer;
		Camera* MainCamera;

	public:
		bool operator()(id_t listenerId, Event& e);

	};

	Scene& CreateGameScene(const ResourcePack& resources);

}