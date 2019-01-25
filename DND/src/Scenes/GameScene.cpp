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
			GameManager::Get().MapManager().LoadTilemap(0);
			GameManager::Get().MapManager().SetCurrentMap(0);
			GameManager::Get().LocalCamera()->Components().AddComponent<PlayerCamera>(&GameManager::Get().MapManager(), GameManager::Get().Players().LocalPlayerObject(), &overlayGameLayer);
			return true;
		});

		gameScene.OnUnload.Subscribe([gameCamera](id_t listenerId, SceneUnloadedEvent& e)
		{
			GameManager::Get().MapManager().UnloadTilemap(GameManager::Get().MapManager().CurrentMapId());
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
		id_t tilemapId = mapManager.AddMap(std::move(tilemap));
	}

	void CreateSceneFromWelcome(const WelcomePacket& packet, const PlayerCharacterInfo& playerInfo)
	{
		GameObject* player = GameManager::Get().Factory().Instantiate(GameManager::Get().Factory().GetPrefab(playerInfo.PrefabId));
		PlayerManager::PlayerInfo myPlayer;
		myPlayer.PlayerId = packet.PlayerId;
		myPlayer.Character.CurrentTile = Tile(0, 0);
		myPlayer.Character.NetworkId = packet.NetworkId;
		myPlayer.Character.PrefabId = playerInfo.PrefabId;
		myPlayer.Character.Stats = player->Components().GetComponent<StatsComponent>().Stats();

		player->Components().GetComponent<TileTransform>().SetCurrentTile(myPlayer.Character.CurrentTile, true);
		player->Components().AddComponent<PlayerController>();
		
		GameManager::Get().Network().Objects().IdentifyObject(packet.NetworkId, player);
		GameManager::Get().Players().SetLocalPlayer(myPlayer, player);

		for (const NetworkPlayerInfo& p : packet.Players)
		{
			if (p.Connection.Address.PrivateEndpoint == GameManager::Get().Network().Address().PrivateEndpoint)
			{
				continue;
			}
			id_t connectionId = GameManager::Get().Network().Connections().GetConnectionId(p.Connection.Address);
			std::function<void(id_t)> sendIntroductionFunc = [myPlayer, p](id_t connectionId)
			{
				IntroductionPacket packet;
				packet.Player.Character = myPlayer.Character;
				packet.Player.PlayerId = myPlayer.PlayerId;
				packet.Player.Connection.Address = GameManager::Get().Network().Address();
				const SocketAddress& address = GameManager::Get().Network().Connections().GetRoutableAddress(connectionId);
				BLT_CORE_INFO("SENDING INTRODUCTION PACKET TO {}", address.ToString());
				GameManager::Get().Network().Server().SendPacket(address, packet);

				GameObject* object = GameManager::Get().Factory().Instantiate(GameManager::Get().Factory().GetPrefab(p.Character.PrefabId));
				object->Components().GetComponent<TileTransform>().SetCurrentTile(p.Character.CurrentTile, true);
				object->Components().GetComponent<StatsComponent>().SetStats(p.Character.Stats);
				object->Components().AddComponent<NetworkController>();
				PlayerManager::PlayerInfo pInfo;
				pInfo.Character = p.Character;
				pInfo.PlayerId = p.PlayerId;
				GameManager::Get().Players().AddPlayer(pInfo.PlayerId, pInfo, object, connectionId);
				GameManager::Get().Network().Objects().IdentifyObject(p.Character.NetworkId, object);
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

		GameManager::Get().Network().Objects().SetNextAvailableNetworkId(packet.NextNetworkId);
		GameManager::Get().Players().SetNextAvailablePlayerId(packet.NextPlayerId);

		GameManager::Get().LocalCamera()->MakeChildOf(player);
		SceneManager::SetCurrentSceneByName("Game");
	}

}