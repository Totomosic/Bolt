#include "App.h"

namespace Blockstream
{

	BlockstreamClient::~BlockstreamClient()
	{
		GameManager::Terminate();
	}

	void BlockstreamClient::Init()
	{
		AppWindow->SetClearColor(Color::SkyBlue);
		scene = SceneManager::CreateScene();
		mainLayer = scene->CreateLayer();
		uiLayer = scene->CreateLayer();
		mainCamera = scene->CreateCamera(Frustum::Perspective(PI / 3, AppWindow->GetFramebuffer().Aspect(), 1.0f, 1000.0f), ProjectionType::Perspective);
		uiCamera = scene->CreateCamera(AppWindow->GetFramebuffer().ViewFrustum(1, 10000), ProjectionType::Orthographic);

		mainLayer->SetActiveCamera(mainCamera);
		uiLayer->SetActiveCamera(uiCamera);

		factory.SetCurrentLayer(mainLayer);

		ResourceManager::Get().Register(std::make_unique<Font>("res/arial.ttf", 24));
		ResourcePack resources = ResourceManager::Get().FetchPack("res/resources.pack");
		ResourceManager::Get().LoadPack(resources);

		backgroundTitleTexture = resources.Resources.at("titleBackground").Id;
		backgroundSpaceTexture = resources.Resources.at("spaceBackground").Id;

		GameManager::Init(this);
		tileMapPrefabId = GameManager::Map().CreatePrefab(factory, TILE_WIDTH, TILE_HEIGHT, TILE_DEPTH);
		CreateTitleScreen();

		RenderSchedule schedule(*scene);
		schedule.AddRenderProcess({});
		SceneRenderer::AddRenderSchedule(schedule);
	}

	void BlockstreamClient::ClearScreen()
	{
		uiLayer->Clear();
		mainLayer->Clear();
	}

	void BlockstreamClient::CreateTitleScreen()
	{
		isPlaying = false;
		ClearScreen();
		uiLayer->UI().Text("Blockstream", Color::Black, Transform({ uiCamera->ViewWidth() / 2, uiCamera->ViewHeight() / 2 + 100, -5 }));

		UIsurface* background = uiLayer->UI().Image(uiCamera->ViewWidth(), uiCamera->ViewHeight(), ResourceManager::Get().GetResource<Texture2D>(backgroundTitleTexture), Transform({ uiCamera->ViewWidth() / 2, uiCamera->ViewHeight() / 2, -50 }));
		UIsurface* title = background->Rectangle(400, 600, Color(150, 150, 150, 200), Transform({ 0, 0, 1 }));

		UIsurface* hostGameButton = title->Rectangle(300, 50, Color(0, 200, 0), Transform({ 0, 0, 1 }));
		hostGameButton->Text("Host", Color::White, Transform({ 0, 0, 1 }));
		hostGameButton->EventHandler().OnClicked.Subscribe([this](id_t eventId, UIClickedEvent& args) -> bool
		{
			CreateHostScreen();
			return false;
		});

		UIsurface* joinGameButton = title->Rectangle(300, 50, Color(0, 200, 0), Transform({ 0, -75, 1 }));
		joinGameButton->Text("Join", Color::White, Transform({ 0, 0, 1 }));
		joinGameButton->EventHandler().OnClicked.Subscribe([this](id_t eventId, UIClickedEvent& args) -> bool
		{
			CreateJoinScreen();
			return false;
		});
	}

	void BlockstreamClient::CreateHostScreen()
	{
		hostString = "";
		isPlaying = false;
		isTypingHost = true;
		ClearScreen();

		hostStringText = uiLayer->UI().Text(hostString, Color::Black, Transform({ uiCamera->ViewWidth() / 2, uiCamera->ViewHeight() / 2 + 100, -5 }));

		UIsurface* playGameButton = uiLayer->UI().Rectangle(300, 50, Color(0, 200, 0), Transform({ uiCamera->ViewWidth() / 2, uiCamera->ViewHeight() / 2 + 0, -5 }));
		playGameButton->Text("Play", Color::White, Transform({ 0, 0, 1 }));
		playGameButton->EventHandler().OnClicked.Subscribe([this](id_t eventId, UIClickedEvent& args) -> bool
		{
			BlockstreamClient* c = this;
			blt::string host = hostString;
			isTypingHost = false;
			CreateGameScreen();
			GameManager::Server().RunAsServer(host);
			GameManager::Server().OnClientConnected.Subscribe([c](id_t eventId, ServerEvent& args) -> bool
			{
				GameManager::Map().RecolorMap();
				GameManager::SetPause(false);
				GameManager::SetTurn(true);
				BLT_CORE_INFO("CLIENT CONNECTED");
				return false;
			});
			GameManager::Server().OnClientDisconnected.Subscribe([c](id_t eventId, ServerEvent& args) -> bool
			{
				c->CreateEndScreen("GameOver, Opponent Disconnected!");
				return false;
			});
			GameManager::Server().OnBlockMoved.Subscribe([c](id_t eventId, ServerEvent& args) -> bool
			{
				GameManager::Map().RecolorMap();
				if (GameManager::EndTile()->IsFlooded())
				{
					GameManager::Server().SendWin();
					std::unique_ptr<ServerEvent> args = std::make_unique<ServerEvent>();
					args->m0 = 0;
					GameManager::Server().OnPlayerWin.Post(std::move(args));
				}
				return false;
			});
			GameManager::Server().OnPlayerWin.Subscribe([c](id_t eventId, ServerEvent& args) -> bool
			{
				blt::string text = "";
				if (args.m0 == 0)
				{
					text = "GameOver, You Won!";
				}
				if (args.m0 == 1)
				{
					text = "GameOver, You Lost!";
				}
				if (args.m0 == 2)
				{
					text = "GameOver, Opponent Disconnected!";
				}
				c->CreateEndScreen(text);
				return false;
			});
			return false;
		});
	}

	void BlockstreamClient::CreateJoinScreen()
	{
		hostString = "";
		isPlaying = false;
		isTypingHost = true;
		ClearScreen();

		hostStringText = uiLayer->UI().Text(hostString, Color::Black, Transform({ uiCamera->ViewWidth() / 2, uiCamera->ViewHeight() / 2 + 100, -5 }));

		UIsurface* playGameButton = uiLayer->UI().Rectangle(300, 50, Color(0, 200, 0), Transform({ uiCamera->ViewWidth() / 2, uiCamera->ViewHeight() / 2 + 0, -5 }));
		playGameButton->Text("Play", Color::White, Transform({ 0, 0, 1 }));
		playGameButton->EventHandler().OnClicked.Subscribe([this](id_t eventId, UIClickedEvent& args) -> bool
		{
			BlockstreamClient* c = this;
			blt::string host = hostString;
			isTypingHost = false;
			CreateGameScreen();
			GameManager::Server().RunAsClient(host);
			GameManager::Server().OnClientConnected.Subscribe([c](id_t eventId, ServerEvent& args) -> bool
			{
				GameManager::Map().RecolorMap();
				GameManager::SetPause(false);
				BLT_CORE_INFO("CLIENT CONNECTED");
				return false;
			});
			GameManager::Server().OnClientDisconnected.Subscribe([c](id_t eventId, ServerEvent& args) -> bool
			{
				c->CreateEndScreen("GameOver, Opponent Disconnected!");
				return false;
			});
			GameManager::Server().OnBlockMoved.Subscribe([c](id_t eventId, ServerEvent& args) -> bool
			{
				GameManager::Map().RecolorMap();
				if (GameManager::EndTile()->IsFlooded())
				{
					GameManager::Server().SendWin();
					std::unique_ptr<ServerEvent> args = std::make_unique<ServerEvent>();
					args->m0 = 0;
					GameManager::Server().OnPlayerWin.Post(std::move(args));
				}
				return false;
			});
			GameManager::Server().OnPlayerWin.Subscribe([c](id_t eventId, ServerEvent& args) -> bool
			{
				blt::string text = "";
				if (args.m0 == 0)
				{
					text = "GameOver, You Won!";
				}
				if (args.m0 == 1)
				{
					text = "GameOver, You Lost!";
				}
				if (args.m0 == 2)
				{
					text = "GameOver, Opponent Disconnected!";
				}
				c->CreateEndScreen(text);
				return false;
			});
			return false;
		});
	}

	void BlockstreamClient::CreateGameScreen()
	{
		isPlaying = true;
		ClearScreen();

		UIsurface* background = uiLayer->UI().Image(uiCamera->ViewWidth(), uiCamera->ViewHeight(), ResourceManager::Get().GetResource<Texture2D>(backgroundSpaceTexture), Transform({ uiCamera->ViewWidth() / 2, uiCamera->ViewHeight() / 2, -9999 }));

		mainCameraAnchor = factory.Instantiate(Transform());
		mainCamera->MakeChildOf(mainCameraAnchor);
		mainCameraAnchor->transform().Reset();
		mainCameraAnchor->transform().Rotate(-PI / 3, Vector3f::Right());
		mainCamera->transform().Reset();
		mainCamera->transform().Translate(0, 0, 200);
		GameManager::Map().RecreateMap(factory, tileMapPrefabId, X_TILES, Y_TILES, WATER_STRENGTH);
		fpsText = uiLayer->UI().Text("fps 60", Color::White, Transform({ 20, uiCamera->ViewHeight() - 30, -5 }), AlignH::Left, AlignV::Center);
	}

	void BlockstreamClient::CreateEndScreen(const blt::string& text)
	{
		GameManager::Server().Shutdown();
		isPlaying = false;
		ClearScreen();
		uiLayer->UI().Text(text, Color::Black, Transform({ uiCamera->ViewWidth() / 2, uiCamera->ViewHeight() / 2, -5 }));

		UIsurface* mainMenuButton = uiLayer->UI().Rectangle(300, 50, Color(0, 200, 0), Transform({ uiCamera->ViewWidth() / 2, uiCamera->ViewHeight() / 2 - 100, -5 }));
		mainMenuButton->EventHandler().OnClicked.Subscribe([this](id_t eventId, UIClickedEvent& args)
		{
			CreateTitleScreen();
			return false;
		});
		mainMenuButton->Text("Play", Color::Black);
	}

	void BlockstreamClient::Tick()
	{
		
	}
		
	void BlockstreamClient::Update()
	{
		if (isTypingHost)
		{
			for (char c : Input::Get().PressedCharacters())
			{
				hostString += c;
			}
			if (Input::Get().KeyPressed(Keycode::Backspace) && hostString.size() > 0)
			{
				hostString.pop_back();
			}
			hostStringText->SetText(hostString);
		}
		if (isPlaying)
		{
			fpsText->SetText("fps " + std::to_string((int)Time::FramesPerSecond()));
			if (Input::Get().MouseButtonDown(MouseButton::Middle))
			{
				float x = Input::Get().RelMousePosition().x;
				float y = Input::Get().RelMousePosition().y;
				mainCameraAnchor->transform().Rotate(-x * ROTATE_X_SENSE, Vector3f::Up(), Space::World);
				mainCameraAnchor->transform().Rotate(y * ROTATE_Y_SENSE, Vector3f::Right(), Space::Local);
			}
			if (Input::Get().MouseButtonPressed(MouseButton::Left))
			{
				Ray mouseRay = mainCamera->NDCToWorldRay(Input::Get().NormalizedMousePosition());
				SGQRayCast raycast = SGQRayCast(mouseRay);
				SGQueryResult result = mainLayer->GameObjects().Query(raycast);
				if (result.MostRelevant != nullptr)
				{
					blt::string reason;
					if (!GameManager::CanDecrementTile(&result.MostRelevant->Components().GetComponent<MapTile>(), &reason))
					{
						BLT_CORE_INFO(reason);
					}
				}
			}
			if (Input::Get().MouseButtonPressed(MouseButton::Right))
			{
				Ray mouseRay = mainCamera->NDCToWorldRay(Input::Get().NormalizedMousePosition());
				SGQueryResult result = mainLayer->GameObjects().Query(SGQRayCast(mouseRay));
				if (result.MostRelevant != nullptr)
				{
					blt::string reason;
					if (!GameManager::CanIncrementTile(&result.MostRelevant->Components().GetComponent<MapTile>(), &reason))
					{
						BLT_CORE_INFO(reason);
					}
				}
			}
			mainCamera->transform().Translate(0, 0, -Input::Get().RelMouseScroll().y * CAMERA_ZOOM_SPEED);
		}
	}

	void BlockstreamClient::Render()
	{
		Graphics::RenderScene();
	}

}