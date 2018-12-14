#include "BoltInclude.h"
#include "Camera\CameraController.h"
#include "Tilemap.h"

namespace DND
{

	class DNDapp : public Application
	{
	public:
		inline static const float CAMERA_SPEED = 500;

	public:
		std::unique_ptr<Tilemap> tilemap;
		Camera* mainCamera;

		GameObject* selectedTileObject;

		Text* fpsText;

	public:
		void Init() override
		{
			Scene* scene = SceneManager::CreateScene();
			Layer* mapLayer = scene->CreateLayer("Map");
			Layer* mapFeatureLayer = scene->CreateLayer("MapFeatures");
			Layer* entityLayer = scene->CreateLayer("Entities");
			Layer* uiLayer = scene->CreateLayer("UI");
			mainCamera = scene->CreateCamera(Projection::Orthographic(-Width() / 2, Width() / 2, -Height() / 2, Height() / 2, 0.0f, 1000.0f));
			Camera* uiCamera = scene->CreateCamera(PrimaryWindow->GetFramebuffer().ViewFrustum(-100, 100), ProjectionType::Orthographic);
			mapLayer->SetActiveCamera(mainCamera);
			mapFeatureLayer->SetActiveCamera(mainCamera);
			entityLayer->SetActiveCamera(mainCamera);
			uiLayer->SetActiveCamera(uiCamera);

			mainCamera->transform().Translate(0, 0, 50);
			mainCamera->Components().AddComponent<CameraController>(CAMERA_SPEED);

			ResourceManager::Register(std::make_unique<Font>("res/arial.ttf", 24));
			ResourcePack resources = ResourceManager::FetchPack("res/resources.pack");
			ResourceManager::LoadPack(resources);

			id_t tilemapShaderId = resources.Resources.at("tilemapShader").Id;
			id_t tilemapTextureId = resources.Resources.at("tilemap").Id;
			id_t grassTextureId = resources.Resources.at("grassTileTexture").Id;
			id_t sandTextureId = resources.Resources.at("sandTileTexture").Id;
			id_t waterTextureId = resources.Resources.at("waterTileTexture").Id;
			id_t pathTextureId = resources.Resources.at("pathTileTexture").Id;

			fpsText = uiLayer->UI().Text("fps 60", Color::White, Transform({ 20, Height() - 30, -5 }), AlignH::Left, AlignV::Center);
			fpsText->Object()->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].RenderOptions.UseDepthTest = false;

			ObjectFactory factory(mapLayer);
			TilemapSetupInfo info;
			info.ShaderId = tilemapShaderId;
			info.GrassTextureId = grassTextureId;
			info.SandTextureId = sandTextureId;
			info.WaterTextureId = waterTextureId;
			info.PathTextureId = pathTextureId;
			tilemap = std::make_unique<Tilemap>(factory, ResourceManager::Get<Texture2D>(tilemapTextureId), 50, info);

			factory.SetCurrentLayer(mapFeatureLayer);
			selectedTileObject = factory.Rectangle(tilemap->Tilesize(), tilemap->Tilesize(), Color(255, 0, 0, 200), Transform({ 0, 0, 1 }));

			RenderSchedule schedule(*scene);
			schedule.AddRenderProcess(RenderProcess());
			SceneRenderer::AddRenderSchedule(schedule);
		}

		void Update() override
		{
			fpsText->SetText("fps " + std::to_string((int)Time::FramesPerSecond()));
			Vector3f mousePosition = Input::MousePosition(mainCamera->ViewWidth(), mainCamera->ViewHeight()) - Vector3f(mainCamera->ViewWidth() / 2.0f, mainCamera->ViewHeight() / 2.0f, 0);
			Vector3f worldPosition = mousePosition + mainCamera->transform().Position();
			if (tilemap->IsTile(worldPosition.x, worldPosition.y))
			{
				const Tilemap::TileInfo& tile = tilemap->GetTileFromWorldPosition(worldPosition.x, worldPosition.y);
				Vector3f tilePos = tilemap->WorldPositionOfTile(tile);
				selectedTileObject->transform().SetLocalPosition(tilePos.x, tilePos.y, selectedTileObject->transform().Position().z);
			}
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
	e.SetWindow(std::make_unique<Window>(1280, 720, "DND"));
	e.SetApplication(std::make_unique<DND::DNDapp>());
	e.Run();
	return 0;
}