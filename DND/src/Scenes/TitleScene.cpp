#include "bltpch.h"
#include "TitleScene.h"

namespace DND
{

	Scene& CreateTitleScene(const ResourcePack& resources)
	{
		Scene& scene = SceneManager::CreateScene("Title");
		Camera* camera = scene.CreateCamera(Projection::Orthographic(0, 1920, 0, 1080, 0, 100));
		Layer& layer = scene.CreateLayer(camera);

		UIsurface& background = layer.UI().Image(camera->ViewWidth(), camera->ViewHeight(), ResourceManager::Get<Texture2D>(resources.GetResourceId("dndBackground")), Transform({ camera->ViewWidth() / 2, camera->ViewHeight() / 2, -50 }));
		UIsurface& titleBack = background.Rectangle(600, 800, Color(30, 30, 30, 240), Transform({ 0, 0, 1 }));
		Text& title = titleBack.Text("Dungeons and Dragons", Color::White, Transform({ 0, 250, 1 }));

		UIsurface& playButton = titleBack.Rectangle(500, 200, Color::Green, Transform({ 0, 0, 1 }));
		playButton.Text("Play", Color::Black);
		playButton.EventHandler().OnClicked.Subscribe([](id_t listenerId, UIClickedEvent& e)
		{
			SceneManager::SetCurrentSceneByName("Character");
			return true;
		});

		RenderSchedule schedule(scene);
		schedule.AddRenderProcess({});
		SceneRenderer::AddRenderSchedule(schedule);
		return scene;
	}

}