#include "bltpch.h"
#include "ServerScene.h"
#include "../App.h"
#include "../GameManager.h"
#include "GameScene.h"

namespace DND
{

	void CreateHostList(UIsurface& background, UIsurface& loadingSymbol, const std::vector<AddressPair>& hosts)
	{
		background.RemoveElement(&loadingSymbol);
		float yOffset = 200;
		for (const AddressPair& address : hosts)
		{
			UIsurface& button = background.Rectangle(300, 50, Color::White, Transform({ 0, yOffset, 0 }));
			button.Text(address.PublicEndpoint.ToString(), Color::Black);
			button.EventHandler().OnClicked.Subscribe([address](id_t listenerId, UIClickedEvent& e)
			{
				GameManager::Get().Network().ConnectTo(address, 10000, [](id_t connectionId)
				{
					if (connectionId != GameObject::InvalidID)
					{
						PlayerCharacterInfo playerInfo;
						playerInfo.PrefabId = GameManager::Get().Prefabs().BlueWizard;
						GameManager::Get().Join(connectionId, playerInfo, CreateSceneFromWelcome);
					}
					else
					{
						BLT_CORE_ERROR("CONNECTION FAILED");
					}
				});
				return true;
			});
			yOffset -= 55;
		}
	}

	void GetHostList(Layer& joinableGamesLayer, id_t loadingIconId)
	{
		joinableGamesLayer.UI().Clear();
		UIsurface& background = joinableGamesLayer.UI().Rectangle(800, 500, Color(100, 100, 100), Transform({ 0, 50, 0 }));
		UIsurface& loadingSymbol = background.Image(50, 50, ResourceManager::Get<Texture2D>(loadingIconId), Transform({ 0, 200, 1 }));
		loadingSymbol.Object()->AddTag("loadingSymbol");
		loadingSymbol.Object()->Components().AddComponent<TriggerComponent>(TriggerComponent::TriggerFunc(), [](GameObject* object)
		{
			object->transform().Rotate(4 * PI * Time::RenderingTimeline().DeltaTime(), -Vector3f::Forward());
		});
		GameManager::Get().Network().GetAllHosts([&background, &loadingSymbol](std::vector<AddressPair> hosts)
		{
			CreateHostList(background, loadingSymbol, hosts);
		});
	}

	void CreateServerScene(DndClient& client, Scene& serverScene, const ResourcePack& resources, Scene& gameScene)
	{
		Projection orthoProj = Projection::Orthographic(0, client.Width(), 0, client.Height(), -100, 100);
		Camera* serverCamera = serverScene.CreateCamera(orthoProj);
		Layer& serverLayer = serverScene.CreateLayer(serverCamera);
		Layer& joinableGamesLayer = serverScene.CreateLayer(serverCamera);

		serverCamera->transform().Translate(-serverCamera->ViewWidth() / 2, -serverCamera->ViewHeight() / 2, 50);

		id_t loadingIconId = resources.GetResourceId("loadingSymbol");

		UIsurface& hostButton = serverLayer.UI().Rectangle(300, 100, Color::Green, Transform({ 0, -280, 0 }));
		hostButton.Text("Host Game");
		hostButton.EventHandler().OnClicked.Subscribe([&client](id_t listenerId, UIClickedEvent& e)
		{
			PlayerCharacterInfo player;
			player.PrefabId = GameManager::Get().Prefabs().Swordsman;
			GameManager::Get().Host(player, CreateSceneFromWelcome);
			return true;
		});

		UIsurface& refreshButton = serverLayer.UI().Rectangle(50, 50, Color(), Transform({ 200, -255, 0 }));
		refreshButton.EventHandler().OnClicked.Subscribe([&joinableGamesLayer, loadingIconId](id_t listenerId, UIClickedEvent& e)
		{
			const PhysObjCollection& objects = joinableGamesLayer.GameObjects().GetGameObjectsByTag("loadingSymbol");
			if (objects.size() == 0)
			{
				GetHostList(joinableGamesLayer, loadingIconId);
			}
			return true;
		});

		serverScene.OnLoad.Subscribe([&client, &joinableGamesLayer, loadingIconId](id_t listenerId, SceneLoadedEvent& e)
		{
			GameManager::Get().Network().Initialize(SocketAddress(client.ADDRESS, client.PORT), [&joinableGamesLayer, loadingIconId](const AddressPair& address)
			{
				GetHostList(joinableGamesLayer, loadingIconId);
			});			

			return true;
		});

		serverScene.OnUnload.Subscribe([&joinableGamesLayer, &serverLayer](id_t listenerId, SceneUnloadedEvent& e)
		{
			joinableGamesLayer.UI().Clear();
			return true;
		});

	}

}