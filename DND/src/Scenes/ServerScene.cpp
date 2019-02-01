#include "bltpch.h"
#include "ServerScene.h"

namespace DND
{

	Scene& CreateServerScene(const ResourcePack& resources)
	{
		Scene& scene = SceneManager::CreateScene("Server");
		Camera* camera = scene.CreateCamera(Projection::Orthographic(0, 1920, 0, 1080, 0, 100));
		Layer& layer = scene.CreateLayer(camera);
		
		UIsurface& joinableGamesBackground = layer.UI().Rectangle(camera->ViewWidth() * 0.75f, camera->ViewHeight() * 0.75f, Color::White, Transform({ camera->ViewWidth() / 2, camera->ViewHeight() * 0.6f, -50 }));
		UIsurface& hostButton = layer.UI().Rectangle(camera->ViewWidth() * 0.3f, camera->ViewHeight() * 0.15f, Color::Green, Transform({ camera->ViewWidth() / 2, camera->ViewHeight() * 0.1125f, -40 }));
		hostButton.Text("Host", Color::Black, Transform({ 0, 0, 1 }));
		hostButton.EventHandler().OnClicked.Subscribe([](id_t listenerId, UIClickedEvent& e)
		{
			// START GAME
			SceneManager::SetCurrentSceneByName("Game");
			return true;
		});

		RenderSchedule schedule(scene);
		schedule.AddRenderProcess({});
		SceneRenderer::AddRenderSchedule(schedule);
		return scene;
	}

}