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
		ObjectFactory* factory;

		GameObject* player;
		GameObject* ground;

		float gameSpeed;
		float score;

	public:
		void Init() override
		{
			PrimaryWindow->SetClearColor(Color::White);
			//PrimaryWindow->EnableVSync();

			Scene* scene = SceneManager::CreateScene();
			Layer* mainLayer = scene->CreateLayer<SceneArray>("Main");

			Camera* mainCamera = scene->CreateCamera(Projection::Orthographic(0, Width(), 0, Height(), 0, 1000));
			mainLayer->SetActiveCamera(mainCamera);

			Font* arial = ResourceManager::Register("Arial", std::make_unique<Font>("res/arial.ttf", 20));

			TextureCreateOptions opt;
			opt.Magnification = MagFilter::Nearest;
			opt.Minification = MinFilter::Nearest;
			Texture2D* trex = ResourceManager::Register("TRex", std::make_unique<Texture2D>(LoadTexture("res/trex_anim.png"), opt));
			Texture2D* groundTexture = ResourceManager::Register("Ground", std::make_unique<Texture2D>(LoadTexture("res/ground.png"), opt));
			Texture2D* cactusLTexture = ResourceManager::Register("LargeCactus", std::make_unique<Texture2D>(LoadTexture("res/large_cactus.png"), opt));
			Texture2D* cactusSTexture = ResourceManager::Register("SmallCactus", std::make_unique<Texture2D>(LoadTexture("res/small_cactus.png"), opt));
			Texture2D* cloudTexture = ResourceManager::Register("Cloud", std::make_unique<Texture2D>(LoadTexture("res/cloud.png"), opt));
			Model* cactusRectangle = ResourceManager::Register("Rectangle", std::make_unique<Model>(RectangleFactory(50, 100)));

			factory = new ObjectFactory(mainLayer);

			Mesh mesh;
			mesh.Models.push_back({ cactusRectangle, Matrix4f::Identity(), { 0 } });
			mesh.Materials[0].Textures.Textures.push_back(cactusSTexture);
			mesh.Materials[0].Textures.Animators[0] = std::make_unique<ZoomAnimator>(6, 1);
			mesh.Materials[0].Shader = Shader::DefaultTexture();

			ObjectPrefab cactusPrefab = ObjectPrefab();
			cactusPrefab.Components().AddComponent(std::make_unique<MeshRenderer>(mesh));
			cactusPrefab.Components().AddComponent(std::make_unique<MotionEngine>(&gameSpeed));
			cactusPrefab.Components().AddComponent(std::make_unique<Collider2D>(Bolt::Rectangle{ { -25, -50 }, { 25, 50 } }));
			factory->AddPrefab(std::move(cactusPrefab));

			Reset(true);

			Time::CreateTimer(2.0f, std::bind(&DinoApp::CreateCactus, this));

			id_t rendererId = Graphics::AddRenderer(std::make_unique<Renderer>(std::make_unique<DefaultRenderMethod>()));
			Graphics::Schedule().RenderPasses.push_back({ Graphics::DefaultFramebuffer(), RenderPass::ALL_LAYERS, Graphics::GetRenderer(rendererId) });
		}

		void Update() override
		{
			gameSpeed += Time::DeltaTime();
			score += gameSpeed / 200 * Time::DeltaTime();
			((ScrollAnimator*)ground->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].Textures.Animators[0].get())->ScrollSpeed() = gameSpeed / Width();

			JumpScript& jump = player->Components().GetComponent<JumpScript>();
			if ((Input::KeyPressed(Keycode::W) || Input::KeyPressed(Keycode::Up)) && jump.CanJump())
			{
				jump.Jump(1100);
			}
			
			if (Input::KeyPressed(Keycode::R))
			{
				Reset(false);
			}

		}

		void Render() override
		{
			//Graphics::Text("fps " + std::to_string((int)Time::FramesPerSecond()), ResourceManager::Get<Font>("Arial"), Width() - 100, Height() - 25, -5, Color::Black, AlignH::Left, AlignV::Top);
			//Graphics::Text("Score: " + std::to_string((int)score), ResourceManager::Get<Font>("Arial"), 20, Height() - 25, -5, Color::Black, AlignH::Left, AlignV::Top);
			Graphics::RenderScene();
		}

		void CreateCactus()
		{
			GameObject* object = factory->Instantiate(factory->GetPrefab(0), Transform(Vector3f(1500, 100 + 100 / 2, -5)));
		}

		void Reset(bool initial)
		{
			gameSpeed = 400;
			score = 0;

			Layer* mainLayer = &SceneManager::CurrentScene().GetLayer("Main");
			mainLayer->Clear();

			ground = factory->Image(Width(), 30, ResourceManager::Get<Texture2D>("Ground"), Transform({ Width() / 2, 100, -6 }));
			ground->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].Textures.Animators[0] = std::make_unique<ScrollAnimator>(Vector2f(-1, 0), 1 / Width() * 400);
			player = factory->Image(44 * 2, 47 * 2, ResourceManager::Get<Texture2D>("TRex"), Transform({ 200, 85 + 47, -5 }));
			player->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].Textures.Animators[0] = std::make_unique<SpriteSheetAnimator>(6, 1, 0.2f);
			player->Components().AddComponent(std::make_unique<JumpScript>(3000));
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