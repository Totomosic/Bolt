#include "BoltEngine.h"

using namespace Bolt;

#include "Collider2D.h"
#include "JumpScript.h"
#include "MotionEngine.h"

namespace DinoGame
{

	class DinoApp : public Application
	{
	public:
		id_t arialFont;
		id_t groundTextureId;
		id_t playerTextureId;
		ObjectFactory factory;

		GameObject* player;
		GameObject* ground;

		float gameSpeed;
		float score;
		bool isPaused = false;

		id_t GAMEOVER_PREFAB;
		id_t CACTUS_PREFAB;

	public:
		void Init() override
		{
			PrimaryWindow->SetClearColor(Color::White);
			//PrimaryWindow->EnableVSync();

			Scene* scene = SceneManager::CreateScene();
			Layer* mainLayer = scene->CreateLayer<SceneArray>("Main");

			Camera* mainCamera = scene->CreateCamera(Projection::Orthographic(0, Width(), 0, Height(), 0, 1000));
			mainLayer->SetActiveCamera(mainCamera);

			arialFont = ResourceManager::Register(std::make_unique<Font>("res/arial.ttf", 20));
			ResourcePtr<Font> arial = ResourceManager::Get<Font>(arialFont);

			ResourcePack resources = ResourceManager::FetchPack("res/dino_resources.pack");
			ResourceManager::LoadPack(resources);

			groundTextureId = resources.Resources.at("ground").Id;
			playerTextureId = resources.Resources.at("trex").Id;
			ResourcePtr<Texture2D> cactusSTexture = ResourceManager::Get<Texture2D>(resources.Resources.at("small_cactus").Id);
			ResourcePtr<Texture2D> gameOverTexture = ResourceManager::Get<Texture2D>(resources.Resources.at("gameover").Id);

			ResourcePtr<Model> cactusRectangle = ResourceManager::Get<Model>(resources.Resources.at("square").Id);
			ResourcePtr<Model> gameOverRectangle = ResourceManager::Get<Model>(ResourceManager::Register(std::make_unique<Model>(RectangleFactory(Width() / 4, Height() / 4))));

			factory = ObjectFactory(mainLayer);

			Mesh cactusMesh;
			cactusMesh.Models.push_back({ cactusRectangle, Matrix4f::Identity(), { 0 } });
			cactusMesh.Materials[0].Textures.Textures.push_back(cactusSTexture);
			cactusMesh.Materials[0].Textures.Animators[0] = std::make_unique<ZoomAnimator>(6, 1);
			cactusMesh.Materials[0].Shader = Shader::DefaultTexture();

			Mesh gameOverMesh = cactusMesh;
			gameOverMesh.Models[0].Model = gameOverRectangle.Clone();
			gameOverMesh.Materials[0].Textures.Textures[0] = gameOverTexture;
			gameOverMesh.Materials[0].Textures.Animators.erase(0);

			ObjectPrefab gameOverPrefab = ObjectPrefab();
			gameOverPrefab.Components().AddComponent(std::make_unique<MeshRenderer>(gameOverMesh));

			ObjectPrefab cactusPrefab = ObjectPrefab();
			cactusPrefab.Components().AddComponent(std::make_unique<MeshRenderer>(cactusMesh));
			cactusPrefab.Components().AddComponent(std::make_unique<MotionEngine>(&gameSpeed, &isPaused));
			cactusPrefab.Components().AddComponent(std::make_unique<Collider2D>(Bolt::Rectangle{ { -25, -50 }, { 25, 50 } }));

			GAMEOVER_PREFAB = factory.AddPrefab(std::move(gameOverPrefab));
			CACTUS_PREFAB = factory.AddPrefab(std::move(cactusPrefab));

			Reset(true);

			Time::CreateTimer(2.0f, std::bind(&DinoApp::CreateCactus, this));

			id_t rendererId = Graphics::AddRenderer(std::make_unique<Renderer>(std::make_unique<DefaultRenderMethod>()));
			Graphics::Schedule().RenderPasses.push_back({ Graphics::DefaultFramebuffer(), RenderPass::ALL_LAYERS, Graphics::GetRenderer(rendererId) });
		}

		void Update() override
		{
			if (Input::KeyPressed(Keycode::G))
			{
				XMLfile xmlFile = Filesystem::OpenXML("test.xml", OpenMode::Write);
				XMLnode node;
				node.Name = "Root";
				XMLserializer writer(node, true);
				writer.Write("Layer", SceneManager::CurrentScene().GetLayer("Main"));
				xmlFile.WriteXML(node);
			}
			if (!isPaused)
			{
				gameSpeed += Time::DeltaTime() * 10;
				score += gameSpeed / 50 * Time::DeltaTime();

				JumpScript& jump = player->Components().GetComponent<JumpScript>();
				if ((Input::KeyPressed(Keycode::W) || Input::KeyPressed(Keycode::Up)) && jump.CanJump())
				{
					jump.Jump(1150);
				}

				if (Input::KeyPressed(Keycode::R))
				{
					Reset(false);
				}

				SGQueryResult result = SceneManager::CurrentScene().GetLayer("Main").Graph().Query(SGQComponents({ typeid(Collider2D) }));
				Collider2D& playerCollider = player->Components().GetComponent<Collider2D>();
				for (GameObject* object : result.GameObjects)
				{
					if (object != player)
					{
						Collider2D& collider = object->Components().GetComponent<Collider2D>();
						bool hit = playerCollider.CollidesWith(collider);
						if (hit)
						{
							GameOver();
						}
					}
				}
			}
			else
			{
				if (Input::MouseButtonPressed(MouseButton::Left))
				{
					Reset(false);
				}
			}
			((ScrollAnimator*)ground->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].Textures.Animators[0].get())->ScrollSpeed() = gameSpeed / Width();
		}

		void Render() override
		{
			//Graphics::Text("fps " + std::to_string((int)Time::FramesPerSecond()), ResourceManager::Get<Font>("Arial"), Width() - 100, Height() - 25, -5, Color::Black, AlignH::Left, AlignV::Top);
			Graphics::Text("Score: " + std::to_string((int)score), ResourceManager::Get<Font>(arialFont), 20, Height() - 25, -5, Color::Black, AlignH::Left, AlignV::Top);
			Graphics::RenderScene();
		}

		void CreateCactus()
		{
			GameObject* object = factory.Instantiate(factory.GetPrefab(CACTUS_PREFAB), Transform(Vector3f(1500, 100 + 100 / 2, -8)));
		}

		void GameOver()
		{
			GameObject* object = factory.Instantiate(factory.GetPrefab(GAMEOVER_PREFAB), Transform(Vector3f(Width() / 2, Height() / 2, -1)));
			Pause();
		}

		void Pause()
		{
			gameSpeed = 0;
			isPaused = true;
		}

		void Reset(bool initial)
		{
			gameSpeed = 400;
			score = 0;
			isPaused = false;

			Layer* mainLayer = &SceneManager::CurrentScene().GetLayer("Main");
			mainLayer->Clear();

			ground = factory.Image(Width(), 30, ResourceManager::Get<Texture2D>(groundTextureId), Transform({ Width() / 2, 100, -10 }));
			ground->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].Textures.Animators[0] = std::make_unique<ScrollAnimator>(Vector2f(-1, 0), 1 / Width() * 400);
			player = factory.Image(44 * 2, 47 * 2, ResourceManager::Get<Texture2D>(playerTextureId), Transform({ 200, 85 + 47, -5 }));
			player->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].Textures.Animators[0] = std::make_unique<SpriteSheetAnimator>(6, 1, 0.2f);
			player->Components().AddComponent(std::make_unique<JumpScript>(3000, &isPaused));
			player->Components().AddComponent(std::make_unique<Collider2D>(Bolt::Rectangle{ { -22 * 2, -47.0f / 2.0f * 2 },{ 22 * 2, 47 / 2.0f * 2 } }));
		}

	};

}

int main()
{
	Engine e;
	WindowCreateInfo info;
	info.Resizable = false;
	e.SetWindow(std::make_unique<Window>(1280, 720, "DinoGame", info));
	e.SetApplication(std::make_unique<DinoGame::DinoApp>());
	e.Run();
	return 0;
}