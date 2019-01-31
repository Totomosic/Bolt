#include "bltpch.h"
#include "App.h"

#include "Scenes/TitleScene.h"
#include "Scenes/CharacterScene.h"
#include "Scenes/ServerScene.h"
#include "Scenes/GameScene.h"

#include "Map/Tilemap.h"
#include "GameManager.h"
#include "Entities/TileMotion.h"
#include "Entities/PlayerController.h"
#include "Camera/PlayerCamera.h"

#include "Networking/NetworkPackets.h"
#include "Networking/NetworkServer.h"
#include "Networking/NetworkManager.h"

namespace DND
{

	void DndClient::Init()
	{
		Scene& titleScene = SceneManager::CreateScene("Title");
		Scene& characterScene = SceneManager::CreateScene("Character");
		Scene& serverScene = SceneManager::CreateScene("Server");
		Scene& gameScene = SceneManager::CreateScene("Game");

		ResourcePack resources = ResourceManager::FetchPack("res/resources.pack");
		ResourceManager::LoadPack(resources);
		ResourceManager::Register(std::make_unique<Font>("res/arial.ttf", 24));

		CreateTitleScene(*this, titleScene, resources);
		CreateCharacterScene(*this, characterScene, resources);
		CreateServerScene(*this, serverScene, resources, gameScene);
		CreateGameScene(*this, gameScene, resources);

		Command setPortCommand("setport", [this](const Command::CommandArgList& args)
		{
			PORT = std::stoi(args[0].c_str());
			BLT_INFO("PORT set to {}", PORT);
		});
		setPortCommand.AddArgument("port", CommandArgType::Int, false);
		CmdDebugger::CmdLine().AddCommand(setPortCommand);

		Command targetPortCommand("targetport", [this](const Command::CommandArgList& args)
		{
			TARGET_PORT = std::stoi(args[0].c_str());
			BLT_INFO("TARGET PORT set to {}", TARGET_PORT);
		});
		targetPortCommand.AddArgument("port", CommandArgType::Int, false);
		CmdDebugger::CmdLine().AddCommand(targetPortCommand);

		Command setAddressCommand("setaddr", [this](const Command::CommandArgList& args)
		{
			ADDRESS = args[0];
			BLT_INFO("ADDRESS set to {}", ADDRESS);
		});
		setAddressCommand.AddArgument("addr", CommandArgType::String, false);
		CmdDebugger::CmdLine().AddCommand(setAddressCommand);

		Command setTargetAddressCommand("targetaddr", [this](const Command::CommandArgList& args)
		{
			TARGET_ADDRESS = args[0];
			BLT_INFO("TARGET ADDRESS set to {}", TARGET_ADDRESS);
		});
		setTargetAddressCommand.AddArgument("addr", CommandArgType::String, false);
		CmdDebugger::CmdLine().AddCommand(setTargetAddressCommand);

		BLT_CORE_INFO("Is big endian, {}", SocketUtil::IsBigEndian());

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

	void DndClient::Tick()
	{
	
	}

	void DndClient::Update()
	{
		if (Input::KeyPressed(Keycode::Esc))
		{
			GameManager::Get().Exit();
		}

		if (Input::KeyPressed(Keycode::P))
		{
			BLT_INFO("PLAYER ID = {}", GameManager::Get().Players().PeekNextPlayerId());
		}
		if (Input::KeyPressed(Keycode::N))
		{
			BLT_INFO("NETWORK ID = {}", GameManager::Get().Network().Objects().PeekNextNetworkId());
		}
		if (Input::KeyPressed(Keycode::K))
		{
			CharacterInfo character;
			character.CurrentTile = Tile(Random::NextInt(0, 20), Random::NextInt(0, 20));
			character.PrefabId = GameManager::Get().Prefabs().Swordsman;
			character.NetworkId = GameManager::Get().Network().Objects().GetNextNetworkId();
			character.Stats = { 30, 30, 1, 1, 1 };
			GameObject* object = GameManager::Get().CreateNetworkCharacter(character, GameManager::Get().Players().LocalPlayer().Player.NetworkId);
			CreateNPCPacket packet;
			packet.Character = character;
			packet.OwnerNetworkId = GameManager::Get().Players().LocalPlayer().Player.NetworkId;
			GameManager::Get().Network().SendPacketToAll(packet);
		}
	}

	void DndClient::Render()
	{
		Graphics::RenderScene();
	}

	void DndClient::Exit()
	{
		if (GameManager::Get().Network().Server().IsRunning())
		{
			GameManager::Get().Exit([this]()
			{
				Application::Exit();
			});
		}
		else
		{
			Application::Exit();
		}
	}

}

int main()
{
	EngineCreateInfo eInfo;
	Engine e(eInfo);
	WindowCreateInfo info;
	info.Title = "Dnd";
	e.SetWindowCreateInfo(info);
	e.SetApplication<DND::DndClient>();
	e.Run();
}