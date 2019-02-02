#include "bltpch.h"
#include "ServerScene.h"
#include "NetworkManager.h"

namespace DND
{

	void GetHostList(UIsurface& region, id_t loadingIconId)
	{
		region.Clear();
		UIsurface& loadingIcon = region.Image(100, 100, ResourceManager::Get<Texture2D>(loadingIconId), Transform({ 0, 300, 1 }));
		loadingIcon.Object()->Components().AddComponent<TriggerComponent>([](GameObject* object)
		{
		
		},
		[](GameObject* object){
			object->transform().Rotate(Quaternion::FromAngleAxis(PI * 4 * Time::RenderingTimeline().DeltaTime(), -Vector3f::Forward()));
		});
		NetworkManager::Get().GetAllHosts(5.0f, [&region](std::vector<AddressPair> hosts)
		{
			region.Clear();
			float yOffset = 300;
			for (const AddressPair& host : hosts)
			{
				UIsurface& joinButton = region.Rectangle(800, 150, Color::Black, Transform({ 0, yOffset, 1 }));
				joinButton.Text(host.PublicEndpoint.ToString(), Color::White, Transform({ 0, 0, 1 }));
				yOffset -= 175;
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
		
		UIsurface& joinableGamesBackground = layer.UI().Rectangle(camera->ViewWidth() * 0.75f, camera->ViewHeight() * 0.75f, Color(200, 200, 200, 230), Transform({ camera->ViewWidth() / 2, camera->ViewHeight() * 0.6f, -50 }));
		UIsurface& hostButton = layer.UI().Rectangle(camera->ViewWidth() * 0.3f, camera->ViewHeight() * 0.15f, Color::Green, Transform({ camera->ViewWidth() / 2, camera->ViewHeight() * 0.1125f, -40 }));
		hostButton.Text("Host", Color::Black, Transform({ 0, 0, 1 }));
		hostButton.EventHandler().OnClicked.Subscribe([](id_t listenerId, UIClickedEvent& e)
		{
			// START GAME
			SceneManager::SetCurrentSceneByName("Game");
			return true;
		});

		UIsurface& refreshButton = layer.UI().Rectangle(50, 50, Color::White, Transform({ camera->ViewWidth() * 0.65f + 35, camera->ViewHeight() * 0.1875f - 25, -40 }));
		refreshButton.EventHandler().OnClicked.Subscribe([&joinableGamesBackground, loadingIconId](id_t listenerId, UIClickedEvent& e)
		{	
			GetHostList(joinableGamesBackground, loadingIconId);
			return true;
		});

		scene.OnLoad.Subscribe([&joinableGamesBackground, loadingIconId](id_t listenerId, SceneLoadedEvent& e)
		{
			GetHostList(joinableGamesBackground, loadingIconId);
			return true;
		});		

		RenderSchedule schedule(scene);
		schedule.AddRenderProcess({});
		SceneRenderer::AddRenderSchedule(schedule);
		return scene;
	}

}