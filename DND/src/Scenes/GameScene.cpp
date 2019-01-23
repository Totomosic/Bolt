#include "bltpch.h"
#include "GameScene.h"
#include "../App.h"
#include "../Spells/BasicSpells.h"
#include "CharPrefabs.h"

#include "../GameManager.h"
#include "../Camera/PlayerCamera.h"
#include "../Entities/PlayerController.h"

namespace DND
{

	void CreateGameScene(DndClient& client, Scene& gameScene, const ResourcePack& resources)
	{
		Projection orthoProj = Projection::Orthographic(0, client.Width(), 0, client.Height(), -100, 100);
		Camera* gameCamera = gameScene.CreateCamera(orthoProj);
		Camera* uiCamera = gameScene.CreateCamera(orthoProj);
		Layer& gameLayer = gameScene.CreateLayer(gameCamera);
		Layer& overlayGameLayer = gameScene.CreateLayer(gameCamera);
		Layer& uiGameLayer = gameScene.CreateLayer(uiCamera);

		gameCamera->transform().Translate(-client.Width() / 2, -client.Height() / 2, 50);

		GameManager::Get().Factory().SetCurrentLayer(overlayGameLayer);
		GameManager::Get().SetLocalCamera(gameCamera);
		CreateCharacterPrefabs(GameManager::Get().Prefabs(), GameManager::Get().Factory(), resources);		

		CreateBasicSpells(resources);

		gameScene.OnLoad.Subscribe([gameCamera, &overlayGameLayer, resources](id_t listenerId, SceneLoadedEvent& e)
		{
			CreateTilemap(resources);
			return true;
		});

		gameScene.OnUnload.Subscribe([gameCamera](id_t listenerId, SceneUnloadedEvent& e)
		{
			if (gameCamera->Components().HasComponent<PlayerCamera>())
			{
				gameCamera->Components().RemoveComponent<PlayerCamera>();
			}
			for (Layer* layer : e.UnloadedScene->GetAllLayers())
			{
				layer->Clear();
			}
			GameManager::Get().GetTilemap().Clear();
			return true;
		});
	}

	void CreateTilemap(const ResourcePack& resources)
	{
		ResourcePtr<Texture2D> tileset = ResourceManager::Get<Texture2D>(resources.GetResourceId("tileset"));
		Image grassTileImage = tileset->GetImage(0, 0, 32, 32);
		Image pathTileImage = tileset->GetImage(0, 32, 32, 32);

		TilemapLayer& layer = GameManager::Get().GetTilemap().AddLayer(1.0f);
		layer.SetTileImages(0, 0, TILEMAP_WIDTH, TILEMAP_HEIGHT, grassTileImage, ResizeFilter::Nearest);
		layer.SetTileImages(25, 0, 4, TILEMAP_HEIGHT, pathTileImage, ResizeFilter::Nearest);
	}

	void CreateSceneFromWelcome(const PlayerCharacterInfo& playerInfo)
	{
		
	}

}