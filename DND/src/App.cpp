#include "bltpch.h"

#include "Map/Tilemap.h"
#include "GameManager.h"
#include "Entities/TileMotion.h"
#include "Entities/PlayerController.h"
#include "Entities/NPCController.h"
#include "Camera/PlayerCamera.h"

#include "Networking/Packets.h"
#include "Networking/NetworkServer.h"
#include "Networking/NetworkManager.h"

namespace DND
{


	class DndClient : public Application
	{
	public:
		int PORT = 12345;
		int TARGET_PORT = 12345;
		blt::string ADDRESS = "localhost";
		blt::string TARGET_ADDRESS = "localhost";

		id_t wizardCharacterPrefabId;

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
			CreateServerScene(serverScene, resources, gameScene);
			CreateGameScene(gameScene, resources);

			Command setPortCommand("setport", [this](const Command::CommandArgList& args)
			{
				PORT = std::stoi(args[0].c_str());
				BLT_CORE_INFO("PORT set to {}", PORT);
			});
			setPortCommand.AddArgument("port", CommandArgType::Int, false);
			CmdDebugger::CmdLine().AddCommand(setPortCommand);

			Command targetPortCommand("targetport", [this](const Command::CommandArgList& args)
			{
				TARGET_PORT = std::stoi(args[0].c_str());
				BLT_CORE_INFO("TARGET PORT set to {}", TARGET_PORT);
			});
			targetPortCommand.AddArgument("port", CommandArgType::Int, false);
			CmdDebugger::CmdLine().AddCommand(targetPortCommand);

			Command setAddressCommand("setaddr", [this](const Command::CommandArgList& args)
			{
				ADDRESS = args[0];
				BLT_CORE_INFO("ADDRESS set to {}", ADDRESS);
			});
			setAddressCommand.AddArgument("addr", CommandArgType::String, false);
			CmdDebugger::CmdLine().AddCommand(setAddressCommand);

			Command setTargetAddressCommand("targetaddr", [this](const Command::CommandArgList& args)
			{
				TARGET_ADDRESS = args[0];
				BLT_CORE_INFO("TARGET ADDRESS set to {}", TARGET_ADDRESS);
			});
			setTargetAddressCommand.AddArgument("addr", CommandArgType::String, false);
			CmdDebugger::CmdLine().AddCommand(setTargetAddressCommand);

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

		void CreateTitleScene(Scene& titleScene, const ResourcePack& resources)
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
			playButton.EventHandler().OnClicked.Subscribe([](id_t listenerId, UIClickedEvent& e)
			{
				SceneManager::SetCurrentSceneByName("Server");
				return true;
			});
		}

		void CreateCharacterScene(Scene& characterScene, const ResourcePack& resources)
		{
			Projection orthoProj = Projection::Orthographic(0, Width(), 0, Height(), -100, 100);
			Camera* characterCamera = characterScene.CreateCamera(orthoProj);
			Layer& characterLayer = characterScene.CreateLayer(characterCamera);
		}

		void CreateServerScene(Scene& serverScene, const ResourcePack& resources, Scene& gameScene)
		{
			Projection orthoProj = Projection::Orthographic(0, Width(), 0, Height(), -100, 100);
			Camera* serverCamera = serverScene.CreateCamera(orthoProj);
			Layer& serverLayer = serverScene.CreateLayer(serverCamera);

			serverCamera->transform().Translate(-serverCamera->ViewWidth() / 2, -serverCamera->ViewHeight() / 2, 50);

			UIsurface& hostButton = serverLayer.UI().Rectangle(300, 100, Color::Green);
			hostButton.Text("Host");
			hostButton.EventHandler().OnClicked.Subscribe([this, resources, &gameScene](id_t listenerId, UIClickedEvent& e)
			{
				GameManager::Get().Network().Server().SetAddress(SocketAddress(ADDRESS, PORT));
				WelcomePacket packet = GameManager::Get().Network().Host();				
				SceneManager::SetCurrentScene(gameScene);
				GameManager::Get().Network().Initialize(packet);
				CreateSceneFromWelcome(packet);
				return true;
			});

			UIsurface& joinButton = serverLayer.UI().Rectangle(300, 100, Color::Red, Transform({ 0, -125, 0 }));
			joinButton.Text("Join");
			joinButton.EventHandler().OnClicked.Subscribe([this, &gameScene](id_t listenerId, UIClickedEvent& e)
			{
				GameManager::Get().Network().Server().SetAddress(SocketAddress(ADDRESS, PORT));
				GameManager::Get().Network().Connect(SocketAddress(TARGET_ADDRESS, TARGET_PORT), [this, &gameScene](WelcomePacket packet)
				{
					SceneManager::SetCurrentScene(gameScene);
					GameManager::Get().Network().Initialize(packet);
					CreateSceneFromWelcome(packet);
				});
				return true;
			});
		}

		void CreateGameScene(Scene& gameScene, const ResourcePack& resources)
		{
			Projection orthoProj = Projection::Orthographic(0, Width(), 0, Height(), -100, 100);
			Camera* gameCamera = gameScene.CreateCamera(orthoProj);
			Layer& gameLayer = gameScene.CreateLayer(gameCamera);
			Layer& overlayGameLayer = gameScene.CreateLayer(gameCamera);

			gameCamera->transform().Translate(-Width() / 2, -Height() / 2, 50);	

			ResourcePtr<Texture2D> downWizard = ResourceManager::Get<Texture2D>(resources.GetResourceId("WizardStaticForward"));
			ResourcePtr<Texture2D> rightWizard = ResourceManager::Get<Texture2D>(resources.GetResourceId("WizardStaticRight"));
			ResourcePtr<Texture2D> upWizard = ResourceManager::Get<Texture2D>(resources.GetResourceId("WizardStaticBackward"));
			ResourcePtr<Texture2D> leftWizard = ResourceManager::Get<Texture2D>(resources.GetResourceId("WizardStaticLeft"));

			ResourcePtr<Model> playerModel = ResourceManager::Get<Model>(ResourceManager::Register(std::make_unique<Model>(RectangleFactory(TILE_WIDTH * 0.9f, TILE_HEIGHT * 1.5f))));
			Mesh wizardCharacterMesh;
			wizardCharacterMesh.Models.push_back({ playerModel });
			wizardCharacterMesh.Materials[0].Shader = Shader::DefaultTexture();

			ObjectPrefab wizardCharacterPrefab;
			wizardCharacterPrefab.Components().AddComponent<MeshRenderer>(wizardCharacterMesh);
			wizardCharacterPrefab.Components().AddComponent<TileTransform>(&GameManager::Get().GetTilemap(), Tile(), Vector3f(0, 15, 0));
			wizardCharacterPrefab.Components().AddComponent<CharacterAnimator>(Direction::Down, upWizard, rightWizard, downWizard, leftWizard);
			wizardCharacterPrefab.Components().AddComponent<TileMotion>(300);

			wizardCharacterPrefabId = GameManager::Get().Network().Factory().AddPrefab(std::move(wizardCharacterPrefab));

			GameManager::Get().Network().Factory().SetCurrentLayer(gameLayer);

			gameScene.OnLoad.Subscribe([gameCamera, &overlayGameLayer, resources, this](id_t listenerId, SceneLoadedEvent& e)
			{
				CreateTilemap(resources);
				if (GameManager::Get().LocalPlayer() != nullptr)
				{
					GameManager::Get().SetLocalCamera(gameCamera);
					gameCamera->MakeChildOf(GameManager::Get().LocalPlayer());
					gameCamera->Components().AddComponent<PlayerCamera>(&GameManager::Get().GetTilemap(), GameManager::Get().LocalPlayer(), &overlayGameLayer);
				}
				return true;
			});

			gameScene.OnUnload.Subscribe([gameCamera](id_t listenerId, SceneUnloadedEvent& e)
			{
				if (gameCamera->Components().HasComponent<PlayerCamera>())
				{
					gameCamera->Components().RemoveComponent<PlayerCamera>();
				}
				for (Layer* layer : e.UnloadedScene->GetAllLayers())
				{
					layer->Clear();
				}
				GameManager::Get().GetTilemap().Clear();
				return true;
			});
		}

		void CreateTilemap(const ResourcePack& resources)
		{
			ResourcePtr<Texture2D> tileset = ResourceManager::Get<Texture2D>(resources.GetResourceId("tileset"));
			Image grassTileImage = tileset->GetImage(0, 0, 32, 32);
			Image pathTileImage = tileset->GetImage(0, 32, 32, 32);

			TilemapLayer& layer = GameManager::Get().GetTilemap().AddLayer(1.0f);
			layer.SetTileImages(0, 0, TILEMAP_WIDTH, TILEMAP_HEIGHT, grassTileImage, ResizeFilter::Nearest);
			layer.SetTileImages(25, 0, 4, TILEMAP_HEIGHT, pathTileImage, ResizeFilter::Nearest);
		}

		void CreateSceneFromWelcome(const WelcomePacket& packet)
		{
			GameManager::Get().Network().SetPlayerId(packet.PlayerId);
			GameManager::Get().Network().SetPlayerPrefab(wizardCharacterPrefabId);
			GameObject* player = GameManager::Get().Network().Factory().Instantiate(GameManager::Get().Network().Factory().GetPrefab(wizardCharacterPrefabId));
			GameManager::Get().Network().IdentifyObject(player, packet.NetworkId, packet.PlayerId);
			GameManager::Get().Network().SetPlayer(player);
			GameManager::Get().SetLocalPlayer(player);
			player->Components().AddComponent<PlayerController>();

			Tile currentTile = Tile(0, 0);
			player->Components().GetComponent<TileTransform>().SetCurrentTile(currentTile, true);

			id_t maxNetworkId = packet.NetworkId;

			IntroductionPacket intro;
			intro.PlayerId = packet.PlayerId;
			intro.Character.CharacterPrefabId = wizardCharacterPrefabId;
			intro.Character.CurrentTile = currentTile;
			intro.Character.NetworkId = packet.NetworkId;

			for (const PlayerInfo& player : packet.Players)
			{
				GameManager::Get().Network().Server().SendPacket(player.Address, intro);
				GameObject* newPlayer = GameManager::Get().Network().Factory().Instantiate(GameManager::Get().Network().Factory().GetPrefab(player.Character.CharacterPrefabId));
				GameManager::Get().Network().IdentifyObject(newPlayer, player.Character.NetworkId, player.PlayerId);
				GameManager::Get().Network().MakeNetworkObject(newPlayer);
				newPlayer->Components().GetComponent<TileTransform>().SetCurrentTile(player.Character.CurrentTile, true);
				maxNetworkId = std::max(maxNetworkId, player.Character.NetworkId);

				NetworkPlayerInfo pl;
				pl.Address = player.Address;
				pl.PlayerId = player.PlayerId;
				pl.PrefabId = player.Character.CharacterPrefabId;
				pl.Player = newPlayer;
				GameManager::Get().Network().AddOtherPlayer(pl);
			}

			GameManager::Get().Network().SetNextAvailableNetworkId(maxNetworkId + 1);
		}

		void Update() override
		{
			if (Input::KeyPressed(Keycode::Esc))
			{
				SceneManager::SetCurrentSceneByName("Title");
				GameManager::Get().Network().Server().Terminate();
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
	WindowCreateInfo info;
	info.Title = "Dnd";
	e.SetWindowCreateInfo(info);
	e.SetApplication<DND::DndClient>();
	e.Run();
}