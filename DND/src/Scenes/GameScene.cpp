#include "bltpch.h"
#include "GameScene.h"
#include "../App.h"
#include "../Spells/BasicSpells.h"
#include "CharPrefabs.h"

#include "../GameManager.h"
#include "../Camera/PlayerCamera.h"
#include "../Entities/PlayerController.h"

#include "../UI/SpellCooldownMenu.h"

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

		GameManager::Get().Network().Factory().SetCurrentLayer(overlayGameLayer);
		GameManager::Get().SetLocalCamera(gameCamera);
		CreateCharacterPrefabs(GameManager::Get().Prefabs(), GameManager::Get().Network().Factory(), resources);		

		CreateBasicSpells(resources);

		GameManager::Get().AddUIMenu(std::make_unique<SpellCooldownMenu>(&uiGameLayer, Vector3f(uiCamera->ViewWidth() / 2, 50, -50), 300, 100, nullptr));

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

	void CreateSceneFromWelcome(const WelcomePacket& packet, const PlayerCharacterInfo& playerInfo)
	{
		GameObject* player = GameManager::Get().Network().Factory().Instantiate(GameManager::Get().Network().Factory().GetPrefab(playerInfo.PrefabId));
		player->Components().AddComponent<PlayerController>();
		Tile currentTile = Tile(Random::NextInt(0, TILEMAP_WIDTH - 1), Random::NextInt(0, TILEMAP_HEIGHT - 1));
		player->Components().GetComponent<TileTransform>().SetCurrentTile(currentTile, true);

		NetworkPlayerInfo myPlayer;
		myPlayer.Address = packet.Address;
		myPlayer.Player = player;
		myPlayer.PlayerId = packet.PlayerId;
		myPlayer.PrefabId = playerInfo.PrefabId;
		GameManager::Get().SetLocalPlayer(myPlayer);

		GameManager::Get().Network().IdentifyObject(player, packet.NetworkId, packet.PlayerId);;
		GameManager::Get().LocalCamera()->MakeChildOf(GameManager::Get().LocalPlayer());
		GameManager::Get().LocalCamera()->Components().AddComponent<PlayerCamera>(&GameManager::Get().GetTilemap(), GameManager::Get().LocalPlayer(), GameManager::Get().Network().Factory().CurrentLayer());

		id_t maxNetworkId = packet.NetworkId;
		id_t maxPlayerId = packet.PlayerId;

		IntroductionPacket intro;
		intro.PlayerId = packet.PlayerId;
		intro.Character.CharacterPrefabId = playerInfo.PrefabId;
		intro.Character.CurrentTile = currentTile;
		intro.Character.NetworkId = packet.NetworkId;
		intro.Character.Stats = player->Components().GetComponent<StatsComponent>().Stats();

		for (const PlayerInfo& player : packet.Players)
		{
			GameManager::Get().Network().Server().SendPacket(player.Address, intro);
			GameObject* newPlayer = GameManager::Get().Network().Factory().Instantiate(GameManager::Get().Network().Factory().GetPrefab(player.Character.CharacterPrefabId));
			newPlayer->Components().GetComponent<TileTransform>().SetCurrentTile(player.Character.CurrentTile, true);
			newPlayer->Components().GetComponent<StatsComponent>().SetStats(player.Character.Stats);

			maxNetworkId = std::max(maxNetworkId, player.Character.NetworkId);
			maxPlayerId = std::max(maxPlayerId, player.PlayerId);

			NetworkPlayerInfo pl;
			pl.Address = player.Address;
			pl.PlayerId = player.PlayerId;
			pl.PrefabId = player.Character.CharacterPrefabId;
			pl.Player = newPlayer;
			GameManager::Get().Network().AddOtherPlayer(pl);

			GameManager::Get().Network().IdentifyObject(newPlayer, player.Character.NetworkId, player.PlayerId);
			GameManager::Get().Network().MakeNetworkPlayer(newPlayer);
		}

		GameManager::Get().Network().SetNextAvailableNetworkId(maxNetworkId + 1);
		GameManager::Get().Network().SetNextAvailablePlayerId(maxPlayerId + 1);
	}

}