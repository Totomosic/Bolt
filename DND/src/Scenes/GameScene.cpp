#include "bltpch.h"
#include "GameScene.h"
#include "../App.h"
#include "../Spells/BasicSpells.h"
#include "CharPrefabs.h"

#include "../GameManager.h"
#include "../Camera/PlayerCamera.h"
#include "../Entities/PlayerController.h"
#include "../Networking/NetworkController.h"

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

		CreateTilemap(resources);
		CreateCharacterPrefabs(GameManager::Get().Prefabs(), GameManager::Get().Animations(), GameManager::Get().Factory(), resources);		
		CreateBasicSpells(resources);

		gameScene.OnLoad.Subscribe([gameCamera, &overlayGameLayer, resources](id_t listenerId, SceneLoadedEvent& e)
		{
			GameManager::Get().MapManager().LoadTilemapAsync(1, []()
			{
				BLT_INFO("TILEMAP 1 LOADED");
			});
			GameManager::Get().MapManager().LoadTilemap(0);
			GameManager::Get().SetCurrentMap(0);
			GameManager::Get().LocalCamera()->Components().AddComponent<PlayerCamera>(&GameManager::Get().MapManager(), GameManager::Get().Players().LocalPlayerObject(), &overlayGameLayer);
			return true;
		});

		gameScene.OnUnload.Subscribe([gameCamera](id_t listenerId, SceneUnloadedEvent& e)
		{
			GameManager::Get().MapManager().UnloadAllTilemaps();
			if (gameCamera->Components().HasComponent<PlayerCamera>())
			{
				gameCamera->Components().RemoveComponent<PlayerCamera>();
			}
			for (Layer* layer : e.UnloadedScene->GetAllLayers())
			{
				layer->Clear();
			}
			return true;
		});
	}

	void CreateTilemap(const ResourcePack& resources)
	{
		ResourcePtr<Texture2D> tileset = ResourceManager::Get<Texture2D>(resources.GetResourceId("Tileset"));
		Image grassTileImage = tileset->GetImage(0, 0, 32, 32);
		Image pathTileImage = tileset->GetImage(0, 32, 32, 32);

		TilemapManager& mapManager = GameManager::Get().MapManager();
		id_t grassTileId = mapManager.TileImages().AddTile(std::move(grassTileImage));
		id_t pathTileId = mapManager.TileImages().AddTile(std::move(pathTileImage));

		Tilemap tilemap(50, 50);
		TilemapLayer& layer = tilemap.AddLayer(1.0f);
		layer.SetRegion(0, 0, 50, 50, grassTileId);
		layer.SetRegion(22, 0, 6, 50, pathTileId);
		layer.SetRegion(0, 22, 50, 6, pathTileId);
		id_t tilemapId = mapManager.AddMap(std::move(tilemap));
		tilemap = Tilemap(50, 50);
		TilemapLayer& newLayer = tilemap.AddLayer(1.0f);
		newLayer.SetRegion(0, 0, 50, 50, pathTileId);
		id_t newTileMapId = mapManager.AddMap(std::move(tilemap));
	}

	void CreateSceneFromWelcome(const WelcomePacket& packet, const PlayerCharacterInfo& playerInfo)
	{
		GameObject* player = GameManager::Get().Factory().Instantiate(GameManager::Get().Factory().GetPrefab(playerInfo.PrefabId));
		PlayerManager::PlayerCharacterInfo myPlayer;
		myPlayer.NetworkId = packet.NetworkId;
		myPlayer.PlayerObject = player;

		Tile currentTile = Tile(0, 0);
		player->Components().AddComponent<CharacterPrefabId>(playerInfo.PrefabId);
		player->Components().GetComponent<TileTransform>().SetCurrentTile(currentTile, true);
		player->Components().AddComponent<PlayerController>();
		
		GameManager::Get().Network().Objects().IdentifyObject(packet.NetworkId, player);
		GameManager::Get().Players().SetLocalPlayer(packet.PlayerId, myPlayer);
		GameManager::Get().Network().Objects().SetNextAvailableNetworkId(packet.NextNetworkId);
		GameManager::Get().Players().SetNextAvailablePlayerId(packet.NextPlayerId);

		for (const NetworkPlayerInfo& p : packet.Players)
		{
			if (p.Connection.Address.PrivateEndpoint == GameManager::Get().Network().Address().PrivateEndpoint)
			{
				continue;
			}
			id_t connectionId = GameManager::Get().Network().Connections().GetConnectionId(p.Connection.Address);
			std::function<void(id_t)> sendIntroductionFunc = [myPlayer, p, currentTile, packet](id_t connectionId)
			{
				IntroductionPacket introPacket;
				introPacket.Player.Character = GameManager::Get().Network().Objects().GetCharacterInfo(packet.NetworkId);
				introPacket.Player.PlayerId = packet.PlayerId;
				introPacket.Player.Connection.Address = GameManager::Get().Network().Address();
				const SocketAddress& address = GameManager::Get().Network().Connections().GetRoutableAddress(connectionId);
				BLT_CORE_INFO("SENDING INTRODUCTION PACKET TO {}", address.ToString());
				GameManager::Get().Network().Server().SendPacket(address, introPacket);

				PlayerManager::PlayerCharacterInfo pInfo;
				pInfo.NetworkId = p.Character.NetworkId;
				pInfo.PlayerObject = GameManager::Get().CreateNetworkObject(p.Character);
				GameManager::Get().Players().AddPlayer(p.PlayerId, pInfo, connectionId);
			};
			if (connectionId == GameObject::InvalidID)
			{
				BLT_CORE_INFO("CREATING NEW CONNECTION");
				GameManager::Get().Network().ConnectTo(p.Connection.Address, 10000, [sendIntro = std::move(sendIntroductionFunc)](id_t connectionId)
				{
					if (connectionId != GameObject::InvalidID)
					{
						sendIntro(connectionId);
					}
					else
					{
						BLT_CORE_ERROR("FAILED TO CONNECT, EXITING...");
						GameManager::Get().Exit();
					}
				});
			}
			else
			{
				BLT_CORE_INFO("CONNECTION ALREADY EXISTED ID = {}", connectionId);
				sendIntroductionFunc(connectionId);
			}
		}
		GameManager::Get().LocalCamera()->MakeChildOf(player);
		SceneManager::SetCurrentSceneByName("Game");
	}

}