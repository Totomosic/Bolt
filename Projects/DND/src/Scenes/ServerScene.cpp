#include "bltpch.h"
#include "ServerScene.h"
#include "NetworkManager.h"

#include "../GlobalState.h"

namespace DND
{

	void GetHostList(UIsurface& region, id_t loadingIconId, EntityNetworkData myCharacter)
	{
		region.Clear();
		UIsurface& loadingIcon = region.Image(100, 100, ResourceManager::Get().GetResource<Texture2D>(loadingIconId), Transform({ 0, 300, 1 }));
		loadingIcon.Object()->Components().AddComponent<TriggerComponent>([](GameObject* object)
		{
		
		},
		[](GameObject* object){
			object->transform().Rotate(Quaternion::FromAngleAxis(PI * 4 * Time::RenderingTimeline().DeltaTime(), -Vector3f::Forward()));
		});
		NetworkManager::Get().GetAllHosts(5.0f, [&region, myCharacter](std::vector<AddressPair> hosts)
		{
			region.Clear();
			float yOffset = 300;
			if (hosts.size() > 0)
			{
				for (const AddressPair& host : hosts)
				{
					UIsurface& joinButton = region.Rectangle(800, 150, Color::Black, Transform({ 0, yOffset, 1 }));
					joinButton.Text(host.PublicEndpoint.ToString(), Color::White, Transform({ 0, 0, 1 }));
					joinButton.EventHandler().OnClicked.Subscribe([myCharacter, host](UIClickedEvent& e)
					{
						GameStartData data;
						data.CharacterData = myCharacter;
						data.IsHosting = false;
						data.Address = host;
						SceneManager::SetCurrentSceneByName("Game", std::move(data));

						ListenerResponse response;
						return response;
					});	
					yOffset -= 175;
				}
			}
			else
			{
				region.Text("No active games found.", Color::Black, Transform({ 0, 300, 1 }));
			}
		},
		[&region]()
		{
			region.Clear();
			region.Text("Unable to connect to server...", Color::Black, Transform({ 0, 300, 1 }));
		});
	}

	Scene& CreateServerScene(const ResourcePack& resources)
	{
		Scene& scene = SceneManager::CreateScene("Server");
		Camera* camera = scene.CreateCamera(Projection::Orthographic(0, 1920, 0, 1080, 0, 100));
		Layer& layer = scene.CreateLayer(camera);

		id_t loadingIconId = resources.GetResourceId("loadingSymbol");
		
		UIelement& sceneElements = layer.UI().AddElement(std::make_unique<UIelement>());
		UIsurface& joinableGamesBackground = layer.UI().Rectangle(camera->ViewWidth() * 0.75f, camera->ViewHeight() * 0.75f, Color(200, 200, 200, 230), Transform({ camera->ViewWidth() / 2, camera->ViewHeight() * 0.6f, -50 }));

		scene.OnLoad.Subscribe([&sceneElements, &joinableGamesBackground, loadingIconId, camera](SceneLoadedEvent& e)
		{
			EntityNetworkData myCharacter;
			myCharacter.MapId = 0;
			myCharacter.NetworkId = 0;
			myCharacter.PrefabId = GlobalState::Prefabs.BlueWizardCharacter;
			myCharacter.TilesPerSecondSpeed = 8;
			myCharacter.TransformData.BottomLeftTile = Tile(0, 0);
			myCharacter.TransformData.Width = 1;
			myCharacter.TransformData.Height = 1;
			
			GetHostList(joinableGamesBackground, loadingIconId, myCharacter);

			UIsurface& hostButton = sceneElements.Rectangle(600, 200, Color::Green, Transform({ camera->ViewWidth() / 2, 125, -40 }));
			hostButton.Text("Host", Color::Black, Transform({ 0, 0, 1 }));
			hostButton.EventHandler().OnClicked.Subscribe([myCharacter](UIClickedEvent& e)
			{
				// START GAME
				GameStartData data;
				data.CharacterData = myCharacter;
				data.IsHosting = true;				
				SceneManager::SetCurrentSceneByName("Game", std::move(data));

				ListenerResponse response;
				return response;
			});

			UIsurface& refreshButton = sceneElements.Rectangle(50, 50, Color::White, Transform({ camera->ViewWidth() / 2 + 335, 200, -40 }));
			refreshButton.EventHandler().OnClicked.Subscribe([&joinableGamesBackground, loadingIconId, camera, myCharacter](UIClickedEvent& e)
			{
				GetHostList(joinableGamesBackground, loadingIconId, std::move(myCharacter));
				ListenerResponse response;
				return response;
			});

			ListenerResponse response;
			return response;
		});

		scene.OnUnload.Subscribe([&sceneElements](SceneUnloadedEvent& e)
		{
			sceneElements.Clear();
			ListenerResponse response;
			return response;
		});

		RenderSchedule schedule(scene);
		schedule.AddRenderProcess({});
		SceneRenderer::AddRenderSchedule(schedule);
		return scene;
	}

}