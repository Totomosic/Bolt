#include "Bolt.h"

#include "Map/Tilemap.h"
#include "GameManager.h"

namespace DND
{

	class DndClient : public Application
	{
	public:

	public:
		void Init() override
		{
			Scene& titleScene = SceneManager::CreateScene("Title");
			Scene& characterScene = SceneManager::CreateScene("Character");
			Scene& serverScene = SceneManager::CreateScene("Server");
			Scene& gameScene = SceneManager::CreateScene("Game");

			ResourcePack resources = ResourceManager::FetchPack("res/resources.pack");
			ResourceManager::LoadPack(resources);
			ResourceManager::Register(std::make_unique<Font>("res/arial.ttf", 24));

			CreateTitleScene(titleScene, resources);
			CreateCharacterScene(characterScene, resources);
			CreateServerScene(serverScene, resources);
			CreateGameScene(gameScene, resources);

			RenderSchedule titleSchedule(titleScene);
			titleSchedule.AddRenderProcess(RenderProcess());
			SceneRenderer::AddRenderSchedule(titleSchedule);
			RenderSchedule characterSchedule(characterScene);
			characterSchedule.AddRenderProcess(RenderProcess());
			SceneRenderer::AddRenderSchedule(characterSchedule);
			RenderSchedule serverSchedule(serverScene);
			serverSchedule.AddRenderProcess(RenderProcess());
			SceneRenderer::AddRenderSchedule(serverSchedule);
			RenderSchedule gameSchedule(gameScene);
			gameSchedule.AddRenderProcess(RenderProcess());
			SceneRenderer::AddRenderSchedule(gameSchedule);
		}

		void CreateTitleScene(Scene& titleScene, ResourcePack& resources)
		{
			Projection orthoProj = Projection::Orthographic(0, Width(), 0, Height(), -100, 100);
			Camera* titleCamera = titleScene.CreateCamera(orthoProj);
			Layer& titleLayer = titleScene.CreateLayer(titleCamera);

			ResourcePtr<Texture2D> backgroundTexture = ResourceManager::Get<Texture2D>(resources.GetResourceId("dndBackground"));
			UIsurface& background = titleLayer.UI().Image(Width(), Height(), backgroundTexture, Transform({ Width() / 2, Height() / 2, -50 }));
			UIsurface& backdrop = background.Rectangle(400, 600, Color(52, 52, 52, 220), Transform({ 0, 0, 5 }));
			Text& title = backdrop.Text("Dungeons and Dragons", Color::White, Transform({ 0, 180, 5 }));

			UIsurface& playButton = backdrop.Rectangle(300, 100, Color::Green);
			playButton.Text("Play");
			playButton.EventHandler().OnClicked.Subscribe([](id_t eventId, UIClickedEvent& e)
			{
				SceneManager::SetCurrentSceneByName("Game");
				return true;
			});
		}

		void CreateCharacterScene(Scene& characterScene, ResourcePack& resources)
		{
			Projection orthoProj = Projection::Orthographic(0, Width(), 0, Height(), -100, 100);
			Camera* characterCamera = characterScene.CreateCamera(orthoProj);
			Layer& characterLayer = characterScene.CreateLayer(characterCamera);
		}

		void CreateServerScene(Scene& serverScene, ResourcePack& resources)
		{
			Projection orthoProj = Projection::Orthographic(0, Width(), 0, Height(), -100, 100);
			Camera* serverCamera = serverScene.CreateCamera(orthoProj);
			Layer& serverLayer = serverScene.CreateLayer(serverCamera);
		}

		void CreateGameScene(Scene& gameScene, ResourcePack& resources)
		{
			Projection orthoProj = Projection::Orthographic(0, Width(), 0, Height(), -100, 100);
			Camera* gameCamera = gameScene.CreateCamera(orthoProj);
			Layer& gameLayer = gameScene.CreateLayer(gameCamera);

			gameCamera->transform().Translate(-Width() / 2, -Height() / 2, 0);

			ResourcePtr<Texture2D> tileset = ResourceManager::Get<Texture2D>(resources.GetResourceId("tileset"));
			Image grassTileImage = tileset->GetImage(0, 0, 32, 32);
			Image pathTileImage = tileset->GetImage(0, 32, 32, 32);

			TilemapLayer& layer = GameManager::Get().GetTilemap().AddLayer(1.0f);
			layer.SetTileImages(0, 0, TILEMAP_WIDTH, TILEMAP_HEIGHT, grassTileImage, ResizeFilter::Nearest);

			gameScene.OnLoad.Subscribe([resources](id_t eventId, SceneLoadedEvent& e)
			{
				ObjectFactory factory(e.LoadedScene->GetLayer(0));
				GameObject* player = factory.Image(TILE_WIDTH * 0.9f, TILE_HEIGHT * 1.5f, ResourceManager::Get<Texture2D>(resources.GetResourceId("WizardStaticForward")), Transform({ 0, 0, 3 }));
				return true;
			});
		}

		void Update() override
		{

		}

		void Render() override
		{
			Graphics::RenderScene();
		}

	};

}

int main()
{
	Engine e;
	WindowCreateInfo info;
	info.Title = "Dnd";
	e.SetWindowCreateInfo(info);
	e.SetApplication<DND::DndClient>();
	e.Run();
}