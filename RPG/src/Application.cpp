#include "BoltEngine.h"

using namespace Bolt;

/*
	2D Side-scrolling spell casting RPG
	Pixel-Art Style
	MMO
	Cities/Towns
	Classes of wizards - Damage, Utility, Travel, etc
*/

#include "GameManager.h"
#include "Entities\PlayerWizard.h"
#include "Tiles\MapTile.h"
#include "Tiles\TileMapLevel.h"
#include "Entities\NPCs\Behaviours\WanderBehaviour.h"
#include "Entities\NPCs\NPCcharacter.h"

#include "Spells\MeteorSpell.h"

namespace RPG
{

	class RPGgame : public Application
	{
	private:

	public:
		void LoadResources()
		{
			TextureCreateOptions pixelArtSettings;
			pixelArtSettings.Magnification = MagFilter::Nearest;
			pixelArtSettings.Wrap = WrapMode::ClampToEdge;

			ResourceManager::Register("Arial24", std::make_unique<Font>("res/Fonts/arial.ttf", 24));

			ResourceManager::Register("Blue_Wizard_Left_Texture", std::make_unique<Texture2D>(LoadTexture("res/Wizards/blue_wizard_left.png", true, false), pixelArtSettings));
			ResourceManager::Register("Blue_Wizard_Right_Texture", std::make_unique<Texture2D>(LoadTexture("res/Wizards/blue_wizard_left.png", true, true), pixelArtSettings));
			ResourceManager::Register("Blue_Wizard_Down_Texture", std::make_unique<Texture2D>(LoadTexture("res/Wizards/blue_wizard_down.png", true, false), pixelArtSettings));
			ResourceManager::Register("Blue_Wizard_Up_Texture", std::make_unique<Texture2D>(LoadTexture("res/Wizards/blue_wizard_up.png", true, false), pixelArtSettings));

			ResourceManager::Register("Purple_Wizard_Left_Texture", std::make_unique<Texture2D>(LoadTexture("res/Wizards/purple_wizard_left.png", true, false), pixelArtSettings));
			ResourceManager::Register("Purple_Wizard_Right_Texture", std::make_unique<Texture2D>(LoadTexture("res/Wizards/purple_wizard_left.png", true, true), pixelArtSettings));
			ResourceManager::Register("Purple_Wizard_Down_Texture", std::make_unique<Texture2D>(LoadTexture("res/Wizards/purple_wizard_down.png", true, false), pixelArtSettings));
			ResourceManager::Register("Purple_Wizard_Up_Texture", std::make_unique<Texture2D>(LoadTexture("res/Wizards/purple_wizard_up.png", true, false), pixelArtSettings));

			ResourceManager::Register("Red_Wizard_Left_Texture", std::make_unique<Texture2D>(LoadTexture("res/Wizards/red_wizard_left.png", true, false), pixelArtSettings));
			ResourceManager::Register("Red_Wizard_Right_Texture", std::make_unique<Texture2D>(LoadTexture("res/Wizards/red_wizard_left.png", true, true), pixelArtSettings));
			ResourceManager::Register("Red_Wizard_Down_Texture", std::make_unique<Texture2D>(LoadTexture("res/Wizards/red_wizard_down.png", true, false), pixelArtSettings));
			ResourceManager::Register("Red_Wizard_Up_Texture", std::make_unique<Texture2D>(LoadTexture("res/Wizards/red_wizard_up.png", true, false), pixelArtSettings));

			ResourceManager::Register("Meteor_Texture", std::make_unique<Texture2D>(LoadTexture("res/meteor_texture.png"), pixelArtSettings));

			Image grassTileImage = LoadTexture("res/grass_tile.png");
			ResourceManager::Register("Grass_Tile_Texture", std::make_unique<Texture2D>(grassTileImage, pixelArtSettings));
			Image sandTileImage = LoadTexture("res/sand_tile.jpg");
			ResourceManager::Register("Sand_Tile_Texture", std::make_unique<Texture2D>(sandTileImage, pixelArtSettings));
			Image waterTileImage = LoadTexture("res/water_tile.png");
			ResourceManager::Register("Water_Tile_Texture", std::make_unique<Texture2D>(waterTileImage, pixelArtSettings));
			Image stonePathTileImage = LoadTexture("res/stone_path_tile.jpg");
			ResourceManager::Register("Stone_Path_Tile_Texture", std::make_unique<Texture2D>(stonePathTileImage, pixelArtSettings));

			ResourceManager::Register("Map_Model", std::make_unique<Model>(RectangleFactory()));
			ResourceManager::Register("Player_Wizard_Model", std::make_unique<Model>(RectangleFactory()));
			ResourceManager::Register("Sprite_Model", std::make_unique<Model>(RectangleFactory()));
		}

		void Init() override
		{
			LoadResources();

			Scene* scene = SceneManager::CreateScene();
			Layer* mainLayer = scene->CreateLayer<SceneArray>("Main");
			Layer* tileMapLayer = scene->CreateLayer<SceneArray>("Tilemap");
			Layer* uiLayer = scene->CreateLayer<SceneArray>("UI");

			CharacterTextureSet blueWizardTextureSet = { ResourceManager::Get<Texture2D>("Blue_Wizard_Left_Texture"), ResourceManager::Get<Texture2D>("Blue_Wizard_Right_Texture"), ResourceManager::Get<Texture2D>("Blue_Wizard_Up_Texture"), ResourceManager::Get<Texture2D>("Blue_Wizard_Down_Texture") };
			CharacterTextureSet purpleWizardTextureSet = { ResourceManager::Get<Texture2D>("Purple_Wizard_Left_Texture"), ResourceManager::Get<Texture2D>("Purple_Wizard_Right_Texture"), ResourceManager::Get<Texture2D>("Purple_Wizard_Up_Texture"), ResourceManager::Get<Texture2D>("Purple_Wizard_Down_Texture") };
			CharacterTextureSet redWizardTextureSet = { ResourceManager::Get<Texture2D>("Red_Wizard_Left_Texture"), ResourceManager::Get<Texture2D>("Red_Wizard_Right_Texture"), ResourceManager::Get<Texture2D>("Red_Wizard_Up_Texture"), ResourceManager::Get<Texture2D>("Red_Wizard_Down_Texture") };

			Camera* mainCamera = scene->CreateCamera();
			mainCamera->SetProjection(Projection::Orthographic(-PrimaryWindow->Width() / 2, PrimaryWindow->Width() / 2, -PrimaryWindow->Height() / 2, PrimaryWindow->Height() / 2, 1, 100));
			mainLayer->SetActiveCamera(mainCamera);
			tileMapLayer->SetActiveCamera(mainCamera);

			Camera* uiCamera = scene->CreateCamera();
			uiLayer->SetActiveCamera(uiCamera);
			uiCamera->SetProjection(Projection::Orthographic(0, PrimaryWindow->Width(), 0, PrimaryWindow->Height(), -100, 100));

			float xTiles = 32;
			float yTiles = 18;
			float xTileSize = PrimaryWindow->Width() / xTiles;
			float yTileSize = PrimaryWindow->Height() / yTiles;
			TileMapLevel* tilemap0000 = GameManager::TileManager.AddTileMap(TileMapLevel(LoadTexture("res/MapTiles/0000.png"), Vector2f(xTileSize, yTileSize), 1));
			TileMapLevel* tilemap0100 = GameManager::TileManager.AddTileMap(TileMapLevel(LoadTexture("res/MapTiles/0100.png"), Vector2f(xTileSize, yTileSize), 1));

			tilemap0000->AddNPC(std::make_unique<NPCcharacter>(&tilemap0000->GetTile(10, 71 - 53), Vector2f(34, 54), 150, CharacterOrientation::Right, redWizardTextureSet, std::make_unique<WanderBehaviour>(), true));
			tilemap0000->AddNPC(std::make_unique<NPCcharacter>(&tilemap0000->GetTile(15, 71 - 53), Vector2f(34, 54), 150, CharacterOrientation::Left, purpleWizardTextureSet, std::make_unique<WanderBehaviour>(), true));

			GameManager::Player = std::make_unique<PlayerWizard>(&tilemap0000->GetTile(0, 71 - 53), Vector2f(34, 54), 250, CharacterOrientation::Right, blueWizardTextureSet);
			GameManager::Player->Create();
			GameManager::Player->SetCurrentTile(&tilemap0000->GetTile(0, 71 - 53));

			GameManager::Camera = std::make_unique<TileCamera>(GameManager::Player.get(), mainCamera, Vector2i(xTiles, yTiles));
			GameManager::Camera->RecreateView(tilemap0000);


			GameManager::TileManager.AddTwoWayConnection(tilemap0000, { Vector2i(127, 71 - 52), Vector2i(0, 71 - 52), CharacterOrientation::Right, tilemap0100 });
			GameManager::TileManager.AddTwoWayConnection(tilemap0000, { Vector2i(127, 71 - 53), Vector2i(0, 71 - 53), CharacterOrientation::Right, tilemap0100 });
			GameManager::TileManager.AddTwoWayConnection(tilemap0000, { Vector2i(127, 71 - 54), Vector2i(0, 71 - 54), CharacterOrientation::Right, tilemap0100 });

			GameManager::TileManager.SetCurrentTileMap(tilemap0000);

			GameManager::Player->Spells().AddSpell(std::make_unique<MeteorSpell>(3, 0, 50, 1.0f, 0.5f, Vector2f(3 * xTileSize, 3 * yTileSize), ResourceManager::Get<Texture2D>("Meteor_Texture")));

			id_t rendererId = Graphics::AddRenderer(std::make_unique<SpriteRenderer2D>());
			id_t defaultRenderer = Graphics::AddRenderer(std::make_unique<Renderer>());
			Graphics::Schedule().RenderPasses.push_back({ &PrimaryWindow->GetFramebuffer(), scene->GetMaskOfLayer("Tilemap"), Graphics::GetRenderer(rendererId) });
			Graphics::Schedule().RenderPasses.push_back({ &PrimaryWindow->GetFramebuffer(), scene->GetMaskOfLayer("Main"), Graphics::GetRenderer(rendererId) });
			Graphics::Schedule().RenderPasses.push_back({ &PrimaryWindow->GetFramebuffer(), scene->GetMaskOfLayer("UI"), Graphics::GetRenderer(defaultRenderer) });
		}

		void Update() override
		{
			GameManager::Player->Update();
			GameManager::TileManager.Update(GameManager::Player.get());
			GameManager::Camera->Update(&GameManager::TileManager.CurrentTileMap());

			Camera* mainCamera = GameManager::Camera->CameraObject();
			float tileSize = 40.0f;

			// Calculate selected Tile
			Vector2f mousePosition = Input::NormalizedMousePosition().xy() * Vector2f(mainCamera->CameraProjection().ViewFrustum.Width(), mainCamera->CameraProjection().ViewFrustum.Height()) / 2.0f + mainCamera->transform().Position().xy();
			int sx = round(mousePosition.x) / tileSize;
			int sy = round(mousePosition.y) / tileSize;
			MapTile* selected = &GameManager::TileManager.CurrentTileMap().GetTile(sx, sy);
			if (selected != GameManager::SelectedTile && selected != nullptr)
			{
				if (GameManager::SelectedTile != nullptr)
				{
					GameManager::SelectedTile->Object()->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].BaseColor = Color::White;
				}
				if (selected->IsCreated())
				{
					selected->Object()->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].BaseColor = Color::Red;
					GameManager::SelectedTile = selected;
				}
			}

			if (Input::MouseButtonDown(MouseButton::Left))
			{
				if (GameManager::SelectedTile != nullptr && GameManager::SelectedTile->ObjectOnTile() != nullptr && GameManager::SelectedTile->ObjectOnTile()->IsAttackable())
				{
					GameManager::SelectedTile->ObjectOnTile()->RemoveHealth(Time::DeltaTime() * 20);
				}
			}
			if (Input::KeyPressed(Keycode::Q))
			{
				GameManager::Player->Spells().Cast(0);
			}

		}

		void Render() override
		{
			blt::string text = "fps " + std::to_string((int)Time::FramesPerSecond());
			const Font* font = ResourceManager::Get<Font>("Arial24");
			Vector2f size = font->SizeOfText(text);
			Graphics::Text(text, font, 1280 - size.x - 5, 720 - size.y / 2 - 5, -5, Color::White, TextAlignmentH::Left, 2);
			Graphics::RenderScene();
		}

	};

}

int main()
{
	Engine e;
	e.SetWindow(std::make_unique<Window>(1280, 720, "RPG"));
	e.SetApplication(std::make_unique<RPG::RPGgame>());
	while (!e.ShouldClose())
	{
		e.UpdateApplication();
	}
	return 0;
}