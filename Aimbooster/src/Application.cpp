#include "BoltEngine.h"

using namespace Bolt;

#include "Target.h"
#include "TargetHitEvent.h"

namespace Aimbooster
{

	class App : public Application
	{
	public:
		int MAX_LIVES = 5;
		float TARGETS_PER_SECOND = 3.5f;
		float TARGET_SIZE = 50;
		float TARGET_LIFETIME = 5;

		Layer* mainLayer;
		Camera* mainCamera;
		Timer* targetTimer;
		ResourcePtr<Font> titleArial;
		ResourcePtr<Font> arial;
		bool isPlaying;

		ObjectFactory factory;
		id_t TARGET_PREFAB;
		id_t TITLE_TARGET_PREFAB;

		Text* timer;
		Text* fpsText;
		Text* scoreText;

		int score = 0;
		int lives = MAX_LIVES;
		float totalTime = 0;

	public:
		void Init() override
		{
			GetWindow().SetClearColor(Color(52, 52, 52));
			isPlaying = false;

			Scene& scene = SceneManager::Get().CreateScene();
			mainLayer = &scene.CreateLayer();
			mainCamera = scene.CreateCamera(GetWindow().GetFramebuffer().ViewFrustum(-100, 100), ProjectionType::Orthographic);
			mainLayer->SetActiveCamera(mainCamera);

			arial = ResourceManager::Get().Fonts().Arial(24);
			titleArial = ResourceManager::Get().Fonts().Arial(42);
			ResourceManager::Get().Fonts().SetDefault(arial);

			ResourceManager::Get().LoadPack("res/resources.pack", [thisPtr{ this }](const ResourcePack& pack)
				{
					ResourceExtractor resources(pack);
					Mesh targetMesh;
					targetMesh.Models.push_back({ ResourcePtr<Model>(BasicModels::Get().Circle()), Matrix4f::Identity(), { 0 } });
					targetMesh.Materials.push_back(ResourceManager::Get().Materials().Texture(resources.GetResourcePtr<Texture2D>("target")));
					ObjectPrefab titleTargetPrefab;
					titleTargetPrefab.Components().AddComponent<MeshRenderer>(targetMesh);
					ObjectPrefab targetPrefab;
					targetPrefab.Components().AddComponent<MeshRenderer>(targetMesh);
					targetPrefab.Components().AddComponent<Target>(thisPtr->TARGET_LIFETIME, thisPtr->TARGET_SIZE, &thisPtr->factory);

					thisPtr->factory = ObjectFactory(*thisPtr->mainLayer);
					thisPtr->TARGET_PREFAB = thisPtr->factory.AddPrefab(std::move(targetPrefab));
					thisPtr->TITLE_TARGET_PREFAB = thisPtr->factory.AddPrefab(std::move(titleTargetPrefab));

					thisPtr->targetTimer = &Time::Get().RenderingTimeline().AddTimer(1 / thisPtr->TARGETS_PER_SECOND, std::bind(&App::CreateTarget, thisPtr));
					thisPtr->targetTimer->Stop();
					EventManager::Get().Subscribe<TargetHitEvent>([thisPtr](TargetHitEvent& args) -> ListenerResponse
						{
							thisPtr->score++;
							ListenerResponse response;
							return response;
						});
					EventManager::Get().Subscribe<TargetFailedEvent>([thisPtr](TargetFailedEvent& args) -> ListenerResponse
						{
							thisPtr->lives--;
							GameObject* marker = thisPtr->factory.Ellipse(5, 5, Color::Black, Transform(args.Position));
							Destroy(marker, 1.0f);
							if (thisPtr->lives <= 0)
							{
								thisPtr->CreateEndScreen();
							}
							ListenerResponse response;
							return response;
						});
					thisPtr->CreateTitleScreen();
				});

			

			RenderSchedule schedule(scene);
			schedule.AddRenderProcess({  });
			SceneRenderer::Get().AddRenderSchedule(schedule);
		}

		void Tick() override
		{
			
		}

		void Update() override
		{
			if (isPlaying)
			{
				totalTime += Time::Get().RenderingTimeline().DeltaTime();
				timer->SetText(TimeToString());
				fpsText->SetText(std::to_string((int)Time::Get().FramesPerSecond()) + " fps");
				scoreText->SetText("Score: " + std::to_string(score));
			}
		}

		void Render() override
		{
			Graphics::Get().RenderScene();
		}	

		void CreateTitleScreen()
		{
			isPlaying = false;
			mainLayer->Clear();
			targetTimer->Stop();
			targetTimer->Reset();
			
			mainLayer->UI().Rectangle(600, mainCamera->ViewHeight(), Color(10, 10, 10), Transform({ mainCamera->ViewWidth() / 2, mainCamera->ViewHeight() / 2, -10 }));
			mainLayer->UI().Text("Aim Booster", titleArial, Color::White, Transform({ mainCamera->ViewWidth() / 2, mainCamera->ViewHeight() - 300, -5 }), AlignH::Center);
			UIsurface& playButton = mainLayer->UI().Rectangle(300, 50, Color(0, 200, 0), Transform({ mainCamera->ViewWidth() / 2, mainCamera->ViewHeight() / 2 - 150, -5 }));
			playButton.Text("Play", Color::White, Transform({ 0, 0, 1 }));

			UIsurface& quitButton = mainLayer->UI().Rectangle(300, 50, Color(200, 0, 0), Transform({ mainCamera->ViewWidth() / 2, mainCamera->ViewHeight() / 2 - 225, -5 }));
			quitButton.Text("Quit", Color::White, Transform({ 0, 0, 1 }));

			playButton.EventHandler().OnClicked.Subscribe([this](UIClickedEvent& args)
			{
				CreateGameScreen();
				ListenerResponse response;
				response.HandledEvent = true;
				return response;
			});

			playButton.EventHandler().OnHoverEntry.Subscribe([](UIHoverEntryEvent& args)
			{
				args.Object->Components().GetComponent<MeshRenderer>().Mesh.Materials[0]->GetLinkContext().GetLink("Color") = Color::Green;
				ListenerResponse response;
				response.HandledEvent = true;
				return response;
			});

			playButton.EventHandler().OnHoverExit.Subscribe([](UIHoverExitEvent& args)
			{
				args.Object->Components().GetComponent<MeshRenderer>().Mesh.Materials[0]->GetLinkContext().GetLink("Color") = Color(0, 200, 0 );
				ListenerResponse response;
				response.HandledEvent = true;
				return response;
			});

			quitButton.EventHandler().OnClicked.Subscribe([this](UIClickedEvent& args)
			{
				Exit();
				ListenerResponse response;
				response.HandledEvent = true;
				return response;
			});

			quitButton.EventHandler().OnHoverEntry.Subscribe([](UIHoverEntryEvent& args)
			{
				args.Object->Components().GetComponent<MeshRenderer>().Mesh.Materials[0]->GetLinkContext().GetLink("Color") = Color::Red;
				return ListenerResponse();
			});

			quitButton.EventHandler().OnHoverExit.Subscribe([](UIHoverExitEvent& args)
			{
				args.Object->Components().GetComponent<MeshRenderer>().Mesh.Materials[0]->GetLinkContext().GetLink("Color") = Color(200, 0, 0);
				return ListenerResponse();
			});
			
		}

		void CreateGameScreen()
		{
			isPlaying = true;
			score = 0;
			lives = MAX_LIVES;
			totalTime = 0;
			mainLayer->Clear();
			targetTimer->Start();
			factory.Rectangle(mainCamera->ViewWidth(), 60, Color(30, 30, 30), Transform({ mainCamera->ViewWidth() / 2, mainCamera->ViewHeight() - 30, -10 }));
			UIsurface& qButton = mainLayer->UI().Rectangle(30, 30, Color(200, 0, 0), Transform({ 30, mainCamera->ViewHeight() - 30, -5 }));
			qButton.Text("<", Color::White, Transform({ 0, 0, 1 }));
			timer = &mainLayer->UI().Text("00:00.0", Color::White, Transform({ 60, mainCamera->ViewHeight() - 30, -5 }), AlignH::Left);
			fpsText = &mainLayer->UI().Text("60 fps", Color::White, Transform({ mainCamera->ViewWidth() - 100, mainCamera->ViewHeight() - 30, -5 }), AlignH::Left);
			scoreText = &mainLayer->UI().Text("Score: 0", Color::White, Transform({ mainCamera->ViewWidth() / 2, mainCamera->ViewHeight() - 30, -5 }), AlignH::Center);

			qButton.EventHandler().OnClicked.Subscribe([this](UIClickedEvent& args)
			{
				CreateTitleScreen();
				return ListenerResponse();
			});

			qButton.EventHandler().OnHoverEntry.Subscribe([](UIHoverEntryEvent& args)
			{
				args.Object->Components().GetComponent<MeshRenderer>().Mesh.Materials[0]->GetLinkContext().GetLink("Color") = Color::Red;
				return ListenerResponse();
			});

			qButton.EventHandler().OnHoverExit.Subscribe([](UIHoverExitEvent& args)
			{
				args.Object->Components().GetComponent<MeshRenderer>().Mesh.Materials[0]->GetLinkContext().GetLink("Color") = Color(200, 0, 0);
				return ListenerResponse();
			});
		}

		void CreateEndScreen()
		{
			isPlaying = false;
			mainLayer->Clear();
			targetTimer->Stop();
			targetTimer->Reset();

			mainLayer->UI().Rectangle(600, mainCamera->ViewHeight(), Color(10, 10, 10), Transform({ mainCamera->ViewWidth() / 2, mainCamera->ViewHeight() / 2, -10 }));
			mainLayer->UI().Text("Game Over", titleArial, Color::White, Transform({ mainCamera->ViewWidth() / 2, mainCamera->ViewHeight() / 2 + 100, -5 }));
			mainLayer->UI().Text("Play time: " + TimeToString(), Color::White, Transform({ mainCamera->ViewWidth() / 2, mainCamera->ViewHeight() / 2 + 0, -5 }));
			mainLayer->UI().Text("Score: " + std::to_string(score), Color::White, Transform({ mainCamera->ViewWidth() / 2, mainCamera->ViewHeight() / 2 - 50, -5 }));
			UIsurface& retryButton = mainLayer->UI().Rectangle(300, 50, Color(0, 200, 0), Transform({ mainCamera->ViewWidth() / 2, mainCamera->ViewHeight() / 2 - 150, -5 }));
			retryButton.Text("Retry", Color::White, Transform({ 0, 0, 1 }));
			retryButton.EventHandler().OnClicked.Subscribe([this](UIClickedEvent& args)
			{
				CreateGameScreen();
				return ListenerResponse();
			});

			retryButton.EventHandler().OnHoverEntry.Subscribe([](UIHoverEntryEvent& args)
			{
				args.Object->Components().GetComponent<MeshRenderer>().Mesh.Materials[0]->GetLinkContext().GetLink("Color") = Color::Green;
				return ListenerResponse();
			});

			retryButton.EventHandler().OnHoverExit.Subscribe([](UIHoverExitEvent& args)
			{
				args.Object->Components().GetComponent<MeshRenderer>().Mesh.Materials[0]->GetLinkContext().GetLink("Color") = Color(0, 200, 0);
				return ListenerResponse();
			});

			UIsurface& menuButton = mainLayer->UI().Rectangle(300, 50, Color(200, 0, 0), Transform({ mainCamera->ViewWidth() / 2, mainCamera->ViewHeight() / 2 - 225, -5 }));
			menuButton.Text("Main Menu", Color::White, Transform({ 0, 0, 1 }));
			menuButton.EventHandler().OnClicked.Subscribe([this](UIClickedEvent& args)
			{
				CreateTitleScreen();
				return ListenerResponse();
			});

			menuButton.EventHandler().OnHoverEntry.Subscribe([](UIHoverEntryEvent& args)
			{
				args.Object->Components().GetComponent<MeshRenderer>().Mesh.Materials[0]->GetLinkContext().GetLink("Color") = Color::Red;
				return ListenerResponse();
			});

			menuButton.EventHandler().OnHoverExit.Subscribe([](UIHoverExitEvent& args)
			{
				args.Object->Components().GetComponent<MeshRenderer>().Mesh.Materials[0]->GetLinkContext().GetLink("Color") = Color(200, 0, 0);
				return ListenerResponse();
			});

			totalTime = 0;
		}

		void CreateTarget()
		{
			GameObject* target = factory.Instantiate(factory.GetPrefab(TARGET_PREFAB), Transform({ Random::NextFloat(0, mainCamera->ViewWidth()), Random::NextFloat(0, mainCamera->ViewHeight() - 60), -20 }));
			target->AddTag("Target");
		}

		blt::string TimeToString()
		{
			double time = (double)totalTime;
			int minutes = (int)(time / 60.0);
			int seconds = (int)(time - 60 * minutes);
			int milliseconds = (int)((time - (minutes * 60 + seconds)) * 1000);
			return std::to_string(minutes / 10) + std::to_string(minutes % 10) + ':' + std::to_string(seconds / 10) + std::to_string(seconds % 10) + '.' + std::to_string(milliseconds / 100);
		}

	};

}

int main()
{	
	EngineCreateInfo createInfo;
	createInfo.WindowInfo.Title = "Aimbooster";
	createInfo.WindowInfo.Samples = 0;
	createInfo.WindowInfo.Resizable = true;
	createInfo.UseSockets = false;
	Engine e(createInfo);
	e.SetApplication<Aimbooster::App>();
	e.Run();
}