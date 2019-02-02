#include "bltpch.h"
#include "GameScene.h"

#include "../NetworkManager.h"
#include "../DndInstance.h"

namespace DND
{

	struct SceneLoadCallback
	{
	public:
		Layer& tilemapLayer;
		TilemapManager tilemaps;
		ObjectFactory factory;
		Camera* camera;

	public:
		bool operator()(id_t listenerId, SceneLoadedEvent& e)
		{
			ObjectFactory f(tilemapLayer);
			GameObject* dndInstance = f.Instantiate();
			dndInstance->Components().AddComponent(std::make_unique<DndInstance>(std::move(tilemaps), std::move(factory), camera));
			return true;
		}

	};

	Scene& CreateGameScene(const ResourcePack& resources)
	{
		ResourcePtr<Texture2D> tileset = ResourceManager::Get<Texture2D>(resources.GetResourceId("Tileset"));

		Scene& scene = SceneManager::CreateScene("Game");
		Camera* camera = scene.CreateCamera(Projection::Orthographic(0, 1920, 0, 1080, 0, 100));
		Layer& tilemapLayer = scene.CreateLayer(camera);
		Layer& entityLayer = scene.CreateLayer(camera);
		camera->transform().Translate(-camera->ViewWidth() / 2, -camera->ViewHeight() / 2, 50);

		TilemapManager tilemaps(tilemapLayer, 50, 50);
		id_t grassTileId = tilemaps.TileImages().AddTile(tileset->GetImage(0, 0, 32, 32));
		id_t pathTileId = tilemaps.TileImages().AddTile(tileset->GetImage(0, 32, 32, 32));
		// Create Tilemaps Here
		Tilemap map0(50, 50);
		TilemapLayer& map0Layer = map0.AddLayer(1.0f);
		map0Layer.SetRegion(0, 0, map0Layer.Width(), map0Layer.Height(), grassTileId);
		map0Layer.SetRegion(22, 0, 6, map0Layer.Height(), pathTileId);
		tilemaps.AddMap(std::move(map0));

		ObjectFactory factory(entityLayer);
		// Create Prefabs Here

		scene.OnLoad.Subscribe<SceneLoadCallback>({ tilemapLayer, std::move(tilemaps), std::move(factory), camera });

		RenderSchedule schedule(scene);
		schedule.AddRenderProcess({});
		SceneRenderer::AddRenderSchedule(schedule);
		return scene;
	}

}