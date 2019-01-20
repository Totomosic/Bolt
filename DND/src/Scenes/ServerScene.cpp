#include "bltpch.h"
#include "ServerScene.h"
#include "../App.h"
#include "../GameManager.h"
#include "GameScene.h"

namespace DND
{

	void CreateServerScene(DndClient& client, Scene& serverScene, const ResourcePack& resources, Scene& gameScene)
	{
		Projection orthoProj = Projection::Orthographic(0, client.Width(), 0, client.Height(), -100, 100);
		Camera* serverCamera = serverScene.CreateCamera(orthoProj);
		Layer& serverLayer = serverScene.CreateLayer(serverCamera);

		serverCamera->transform().Translate(-serverCamera->ViewWidth() / 2, -serverCamera->ViewHeight() / 2, 50);

		UIsurface& hostButton = serverLayer.UI().Rectangle(300, 100, Color::Green);
		hostButton.Text("Host");
		hostButton.EventHandler().OnClicked.Subscribe([&client](id_t listenerId, UIClickedEvent& e)
		{
			PlayerCharacterInfo player;
			player.PrefabId = GameManager::Get().Prefabs().Swordsman;
			GameManager::Get().Host(SocketAddress(client.ADDRESS, client.PORT), player, CreateSceneFromWelcome);
			return true;
		});

		UIsurface& joinButton = serverLayer.UI().Rectangle(300, 100, Color::Red, Transform({ 0, -125, 0 }));
		joinButton.Text("Join");
		joinButton.EventHandler().OnClicked.Subscribe([&client, &gameScene](id_t listenerId, UIClickedEvent& e)
		{
			if (client.ADDRESS != client.TARGET_ADDRESS || client.PORT != client.TARGET_PORT)
			{
				PlayerCharacterInfo player;
				player.PrefabId = GameManager::Get().Prefabs().BlueWizard;
				GameManager::Get().Join(SocketAddress(client.ADDRESS, client.PORT), SocketAddress(client.TARGET_ADDRESS, client.TARGET_PORT), player, CreateSceneFromWelcome);
			}
			else
			{
				BLT_CORE_WARN("ATTEMPTED TO JOIN SELF");
			}
			return true;
		});
	}

}