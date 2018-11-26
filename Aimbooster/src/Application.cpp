#include "BoltEngine.h"

using namespace Bolt;

#include "Target.h"

namespace Aimbooster
{

	class App : public Application
	{
	public:
		float TARGETS_PER_SECOND = 2;
		float TARGET_SIZE = 50;
		float TARGET_LIFETIME = 5;

		Layer* mainLayer;
		Camera* mainCamera;
		id_t targetTimer;
		id_t titleArial;
		id_t arial;
		bool isPlaying;

		ObjectFactory factory;
		id_t TARGET_PREFAB;
		id_t TITLE_TARGET_PREFAB;

		Text* timer;
		Text* fpsText;
		Text* scoreText;

		int score = 0;

	public:
		void Init() override
		{
			PrimaryWindow->SetClearColor(Color(52, 52, 52));
			isPlaying = false;

			Scene* scene = SceneManager::CreateScene();
			mainLayer = scene->CreateLayer<SceneArray>("Main");
			mainCamera = scene->CreateCamera(PrimaryWindow->GetFramebuffer().ViewFrustum(-0.1f, 100), ProjectionType::Orthographic);
			mainLayer->SetActiveCamera(mainCamera);

			arial = ResourceManager::Register(std::make_unique<Font>("res/arial.ttf", 24));
			titleArial = ResourceManager::Register(std::make_unique<Font>("res/arial.ttf", 42));			

			ResourcePack resources = ResourceManager::FetchPack("res/resources.pack");
			ResourceManager::LoadPack(resources);

			Mesh targetMesh;
			targetMesh.Models.push_back({ ResourcePtr<Model>(new Model(EllipseFactory(2, 2)), true), Matrix4f::Identity(), { 0 } });
			targetMesh.Materials[0].BaseColor = Color::White;
			targetMesh.Materials[0].Textures.Textures.push_back(ResourceManager::Get<Texture2D>(resources.Resources.at("target").Id));
			targetMesh.Materials[0].Shader = Shader::DefaultTexture();
			ObjectPrefab titleTargetPrefab;
			titleTargetPrefab.Components().AddComponent<MeshRenderer>(targetMesh);
			ObjectPrefab targetPrefab;
			targetPrefab.Components().AddComponent<MeshRenderer>(targetMesh);
			targetPrefab.Components().AddComponent<Target>(TARGET_LIFETIME, TARGET_SIZE, &factory);

			factory = ObjectFactory(mainLayer);
			TARGET_PREFAB = factory.AddPrefab(std::move(targetPrefab));
			TITLE_TARGET_PREFAB = factory.AddPrefab(std::move(titleTargetPrefab));

			targetTimer = Time::CreateTimer(1 / TARGETS_PER_SECOND, std::bind(&App::CreateTarget, this));
			Time::GetTimer(targetTimer)->Stop();
			EventManager::Subscribe(TARGET_HIT_EVENT, [this](id_t eventId, const EventArgs* args)
			{
				this->score++;
			});
			EventManager::Subscribe(TARGET_FAILED_EVENT, [this](id_t eventId, const EventArgs* args)
			{
				this->score--;
			});

			id_t renderer = Graphics::AddRenderer(std::make_unique<Renderer>(std::make_unique<DefaultRenderMethod>()));
			Graphics::Schedule().RenderPasses.push_back({ Graphics::DefaultFramebuffer(), RenderPass::ALL_LAYERS, Graphics::GetRenderer(renderer), {}, {  } });

			CreateTitleScreen();
		}

		void Tick() override
		{
			
		}

		void Update() override
		{
			if (Input::KeyPressed(Keycode::R))
			{
				CreateGameScreen();
			}
			if (Input::KeyPressed(Keycode::T))
			{
				CreateTitleScreen();
			}
			if (isPlaying)
			{
				timer->SetText(TimeToString());
				fpsText->SetText(std::to_string((int)Time::FramesPerSecond()) + " fps");
				scoreText->SetText("Score: " + std::to_string(score));
			}
		}

		void Render() override
		{
			Graphics::RenderScene();
		}	

		void CreateTitleScreen()
		{
			isPlaying = false;
			mainLayer->Clear();
			Time::GetTimer(targetTimer)->Stop();
			Time::GetTimer(targetTimer)->Reset();
			
			mainLayer->UI().AddElement<Text>("Aim Booster", ResourceManager::Get<Font>(titleArial), Color::White, Transform({ mainCamera->ViewWidth() / 2, mainCamera->ViewHeight() - 300, -5 }), AlignH::Center);
			factory.Instantiate(factory.GetPrefab(TITLE_TARGET_PREFAB), Transform({ 300, 400, -5 }, Quaternion::Identity(), { 50, 50, 1 }));
			UIsurface* playButton = mainLayer->UI().Rectangle(300, 75, Color(0, 200, 0), Transform({ mainCamera->ViewWidth() / 2, mainCamera->ViewHeight() - 450, -5 }));
			playButton->Text("Play", Color::White, Transform({ 0, 0, 1 }, Quaternion::Identity(), { 1.5f, 1.5f, 1.0f }));

			UIsurface* quitButton = mainLayer->UI().Rectangle(150, 50, Color(200, 0, 0), Transform({ mainCamera->ViewWidth() / 2, mainCamera->ViewHeight() - 550, -5 }));
			quitButton->Text("Quit", Color::White, Transform({ 0, 0, 1 }));

			playButton->EventHandler().OnClicked.Subscribe([this](id_t eventId, const UIEventArgs* args)
			{
				CreateGameScreen();
			});

			playButton->EventHandler().OnHoverEntry.Subscribe([](id_t eventId, const UIEventArgs* args)
			{
				args->Object->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].BaseColor = Color::Green;
			});

			playButton->EventHandler().OnHoverExit.Subscribe([](id_t eventId, const UIEventArgs* args)
			{
				args->Object->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].BaseColor = Color(0, 200, 0 );
			});

			quitButton->EventHandler().OnClicked.Subscribe([this](id_t eventId, const UIEventArgs* args)
			{
				PrimaryWindow->Close();
			});
			
		}

		void CreateGameScreen()
		{
			isPlaying = true;
			score = 0;
			Time::Reset();
			mainLayer->Clear();
			Time::GetTimer(targetTimer)->Start();
			factory.Rectangle(mainCamera->ViewWidth(), 60, Color(30, 30, 30), Transform({ mainCamera->ViewWidth() / 2, mainCamera->ViewHeight() - 30, -10 }));
			timer = mainLayer->UI().Text("00:00.0", ResourceManager::Get<Font>(arial), Color::White, Transform({ 20, mainCamera->ViewHeight() - 30, -5 }), AlignH::Left);
			fpsText = mainLayer->UI().Text("60 fps", Color::White, Transform({ mainCamera->ViewWidth() - 100, mainCamera->ViewHeight() - 30, -5 }), AlignH::Left);
			scoreText = mainLayer->UI().Text("Score: 0", Color::White, Transform({ mainCamera->ViewWidth() / 2, mainCamera->ViewHeight() - 30, -5 }), AlignH::Center);
		}

		void CreateTarget()
		{
			GameObject* target = factory.Instantiate(factory.GetPrefab(TARGET_PREFAB), Transform({ Random::NextFloat(0, mainCamera->ViewWidth()), Random::NextFloat(0, mainCamera->ViewHeight() - 60), -20 }));
		}

		blt::string TimeToString()
		{
			double time = Time::CurrentTime();
			int minutes = (int)(time / 60.0);
			int seconds = (int)(time - 60 * minutes);
			int milliseconds = (int)((time - (minutes * 60 + seconds)) * 1000);
			return std::to_string(minutes / 10) + std::to_string(minutes % 10) + ':' + std::to_string(seconds / 10) + std::to_string(seconds % 10) + '.' + std::to_string(milliseconds / 100);
		}

	};

}

int main()
{
	Engine e;
	WindowCreateInfo createInfo;
	createInfo.Samples = 4;
	createInfo.Resizable = true;
	e.SetWindow(std::make_unique<Window>(1280, 720, "Aim Booster", createInfo));
	e.SetApplication(std::make_unique<Aimbooster::App>());
	e.Run();
}