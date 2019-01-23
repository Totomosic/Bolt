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
		Layer& joinableGamesLayer = serverScene.CreateLayer(serverCamera);

		serverCamera->transform().Translate(-serverCamera->ViewWidth() / 2, -serverCamera->ViewHeight() / 2, 50);


		serverScene.OnLoad.Subscribe([&client, &serverLayer, &joinableGamesLayer](id_t listenerId, SceneLoadedEvent& e)
		{
			GameManager::Get().Network().Initialize(SocketAddress(client.ADDRESS, client.PORT), [&client, &serverLayer](const AddressPair& address)
			{
				UIsurface& hostButton = serverLayer.UI().Rectangle(300, 100, Color::Green, Transform({ 0, -200, 0 }));
				hostButton.Text("Host");
				hostButton.EventHandler().OnClicked.Subscribe([&client](id_t listenerId, UIClickedEvent& e)
				{
					PlayerCharacterInfo player;
					player.PrefabId = GameManager::Get().Prefabs().Swordsman;
					return true;
				});
			});

			GameManager::Get().Network().GetAllHosts([&joinableGamesLayer](std::vector<AddressPair> hosts)
			{
				float yOffset = 200;
				for (AddressPair& address : hosts)
				{
					UIsurface& button = joinableGamesLayer.UI().Rectangle(300, 50, Color::White, Transform({ 0, yOffset, 0 }));
					button.Text(address.PublicEndpoint.ToString(), Color::Black);
					button.EventHandler().OnClicked.Subscribe([address](id_t listenerId, UIClickedEvent& e)
					{
						GameManager::Get().Network().ConnectTo(address, [](id_t connectionId)
						{
							BLT_CORE_WARN("CONNECTED!!!!");
						});
						return true;
					});
				}
			});

			return true;
		});

		serverScene.OnUnload.Subscribe([&joinableGamesLayer, &serverLayer](id_t listenerId, SceneUnloadedEvent& e)
		{
			serverLayer.UI().Clear();
			joinableGamesLayer.UI().Clear();
			return true;
		});

	}

}