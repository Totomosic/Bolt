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
#include "Entities/NPCController.h"
#include "Camera/PlayerCamera.h"

#include "Networking/Packets.h"
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

	void DndClient::Tick()
	{
	
	}

	void DndClient::Update()
	{
		if (Input::KeyPressed(Keycode::Esc))
		{
			GameManager::Get().Exit();
		}

		if (GameManager::Get().LocalPlayer() != nullptr)
		{
			GameManager::Get().Update();
		}
	}

	void DndClient::Render()
	{
		Graphics::RenderScene();
	}

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