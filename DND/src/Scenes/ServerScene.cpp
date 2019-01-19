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
		hostButton.EventHandler().OnClicked.Subscribe([&client, resources, &gameScene](id_t listenerId, UIClickedEvent& e)
		{
			GameManager::Get().Network().SetAddress(SocketAddress(client.ADDRESS, client.PORT));
			WelcomePacket packet = GameManager::Get().Network().Host();
			GameManager::Get().Initialize(packet, gameScene);
			CreateSceneFromWelcome(packet, GameManager::Get().Prefabs().BlueWizard);
			return true;
		});

		UIsurface& joinButton = serverLayer.UI().Rectangle(300, 100, Color::Red, Transform({ 0, -125, 0 }));
		joinButton.Text("Join");
		joinButton.EventHandler().OnClicked.Subscribe([&client, &gameScene](id_t listenerId, UIClickedEvent& e)
		{
			GameManager::Get().Network().SetAddress(SocketAddress(client.ADDRESS, client.PORT));
			if (client.TARGET_PORT != client.PORT || client.TARGET_ADDRESS != client.ADDRESS)
			{
				GameManager::Get().Network().Connect(SocketAddress(client.TARGET_ADDRESS, client.TARGET_PORT), [&client, &gameScene](WelcomePacket packet)
				{
					GameManager::Get().Initialize(packet, gameScene);
					CreateSceneFromWelcome(packet, GameManager::Get().Prefabs().BlueWizard);
				});
			}
			else
			{
				BLT_CORE_ERROR("TARGET ADDRESS WAS SAME AS BOUND ADDRESS");
			}
			return true;
		});
	}

}