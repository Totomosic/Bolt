#include "bltpch.h"
#include "GameScene.h"

#include "../DndInstance.h"

namespace DND
{

	Scene& CreateGameScene(const ResourcePack& resources)
	{
		ResourcePtr<Texture2D> tileset = ResourceManager::Get<Texture2D>(resources.GetResourceId("Tileset"));

		Scene& scene = SceneManager::CreateScene("Game");
		Camera* camera = scene.CreateCamera(Projection::Orthographic(0, 1920, 0, 1080, 0, 100));
		Layer& layer = scene.CreateLayer(camera);
		camera->transform().Translate(-camera->ViewWidth() / 2, -camera->ViewHeight() / 2, 50);

		scene.OnLoad.Subscribe([&layer, tileset](id_t listenerId, SceneLoadedEvent& e)
		{
			ObjectFactory factory(layer);
			GameObject* dndInstance = factory.Instantiate();
			dndInstance->Components().AddComponent<DndInstance>(layer, tileset);
			return true;
		});

		RenderSchedule schedule(scene);
		schedule.AddRenderProcess({});
		SceneRenderer::AddRenderSchedule(schedule);
		return scene;
	}

}