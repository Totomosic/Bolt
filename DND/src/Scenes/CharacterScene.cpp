#include "bltpch.h"
#include "CharacterScene.h"

namespace DND
{

	Scene& CreateCharacterScene(const ResourcePack& resources)
	{
		Scene& scene = SceneManager::CreateScene("Character");
		Camera* camera = scene.CreateCamera(Projection::Orthographic(0, 1920, 0, 1080, 0, 100));
		Layer& layer = scene.CreateLayer(camera);

		scene.OnLoad.Subscribe([](id_t listenerId, SceneLoadedEvent& e)
		{
			SceneManager::SetCurrentSceneByName("Server");
			return true;
		});

		RenderSchedule schedule(scene);
		schedule.AddRenderProcess({});
		SceneRenderer::AddRenderSchedule(schedule);
		return scene;
	}

}