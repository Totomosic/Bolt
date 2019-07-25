#include "bltpch.h"
#include "GameScene.h"
#include "ServerScene.h"

#include "../GlobalState.h"
#include "../DndInstance.h"

namespace DND
{

	Scene& CreateGameScene(const ResourcePack& resources)
	{
		Scene& scene = SceneManager::CreateScene("Game");
		Camera* camera = scene.CreateCamera(Projection::Orthographic(0, 1920, 0, 1080, 0, 100));
		Layer& tilemapLayer = scene.CreateLayer(camera);
		Layer& entityLayer = scene.CreateLayer(camera);
		camera->transform().Translate(0, 0, 50);

		GlobalState::MapManager.SetMapLayer(tilemapLayer);
		GlobalState::Factory.SetCurrentLayer(entityLayer);

		scene.OnLoad.Subscribe([](SceneLoadedEvent& e)
		{
			GameStartData& data = *(GameStartData*)e.LoadData.get();

			GameObject* dndInstance = GlobalState::Factory.Instantiate();
			DndInstance& instance = dndInstance->Components().AddComponent<DndInstance>(GlobalState::MapManager, GlobalState::Factory);
			if (data.IsHosting)
			{
				instance.StartGame(data.CharacterData);
			} 
			else
			{
				instance.StartGame(data.Address, data.CharacterData, [addr = data.Address]()
				{
					BLT_ERROR("CONNECTION TO {} TIMED OUT", addr.PrivateEndpoint);
					SceneManager::SetCurrentSceneByName("Server");
				});
			}

			ListenerResponse response;
			return response;
		});

		scene.OnUnload.Subscribe([&entityLayer](SceneUnloadedEvent& e)
		{
			entityLayer.Clear();
			ListenerResponse response;
			return response;
		});

		RenderSchedule schedule(scene);
		schedule.AddRenderProcess({});
		SceneRenderer::AddRenderSchedule(schedule);
		return scene;
	}

}