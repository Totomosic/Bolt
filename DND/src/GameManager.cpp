#include "bltpch.h"
#include "GameManager.h"

#include "Camera/PlayerCamera.h"

namespace DND
{

	GameManager* GameManager::s_Instance = nullptr;

	GameManager& GameManager::Get()
	{
		if (s_Instance == nullptr)
		{
			s_Instance = new GameManager(SceneManager::GetSceneByName("Game").GetLayer(0));
		}
		return *s_Instance;
	}

	void GameManager::Terminate()
	{
		delete s_Instance;
		s_Instance = nullptr;
	}

	GameManager::GameManager(Layer& layer)
		: m_LocalCamera(nullptr), m_LocalPlayer(nullptr), m_Tilemap(layer, TILEMAP_WIDTH, TILEMAP_HEIGHT, TILE_WIDTH, TILE_HEIGHT), m_Prefabs(), m_Network(), m_Spells(), m_UImenus(), m_ActiveFunctions()
	{
	
	}

	void GameManager::Host(const SocketAddress& address, PlayerCharacterInfo player, const std::function<void(const WelcomePacket&, const PlayerCharacterInfo&)>& loadSceneCallback)
	{
		Network().Server().SetAddress(address);
		Network().Server().Bind();
		Network().Initialize();
		WelcomePacket packet = Network().Host();
		SceneManager::SetCurrentSceneByName("Game");
		loadSceneCallback(packet, player);
		Initialize();
	}

	void GameManager::Join(const SocketAddress& address, const SocketAddress& toAddress, PlayerCharacterInfo player, const std::function<void(const WelcomePacket&, const PlayerCharacterInfo&)>& loadSceneCallback)
	{
		Network().Server().SetAddress(address);
		Network().Server().Bind();
		Network().Initialize();
		Network().Connect(toAddress, [this, player, loadSceneCallback](WelcomePacket packet)
		{
			SceneManager::SetCurrentSceneByName("Game");
			loadSceneCallback(packet, player);
			Initialize();
		});
	}

	void GameManager::Initialize()
	{
		for (auto& menu : m_UImenus)
		{
			menu->CreateMenu();
		}
	}

	void GameManager::Exit()
	{
		Exit([]() {});
	}

	void GameManager::Exit(const std::function<void()>& callback)
	{
		m_LocalPlayer = nullptr;
		Network().Exit([this, callback]()
		{
			for (auto& menu : m_UImenus)
			{
				menu->DestroyMenu();
			}
			for (Timer* func : m_ActiveFunctions)
			{
				Time::RenderingTimeline().RemoveFunction(func);
			}
			SceneManager::SetCurrentSceneByName("Title");
			callback();
		});
	}

	void GameManager::Update()
	{
		for (auto& menu : m_UImenus)
		{
			menu->Update();
		}
	}

	Camera* GameManager::LocalCamera() const
	{
		return m_LocalCamera;
	}

	GameObject* GameManager::LocalPlayer() const
	{
		return m_LocalPlayer;
	}

	void GameManager::SetLocalCamera(Camera* camera)
	{
		m_LocalCamera = camera;
	}

	void GameManager::SetLocalPlayer(const NetworkPlayerInfo& player)
	{
		m_LocalPlayer = player.Player;
		Network().SetPlayer(player);
	}

	GameStateObjects GameManager::GetStateObjects()
	{
		return { &Network(), &Network().Factory(), &GetTilemap(), LocalPlayer() };
	}

	GameState GameManager::GetGameState()
	{
		GameState state = { GetStateObjects() };
		state.SelectedTile = CurrentlySelectedTile();
		return state;
	}

	Tile GameManager::CurrentlySelectedTile() const
	{
		BLT_ASSERT(m_LocalCamera != nullptr, "No local camera");
		return m_LocalCamera->Components().GetComponent<PlayerCamera>().SelectedTile();
	}

	PrefabList& GameManager::Prefabs()
	{
		return m_Prefabs;
	}

	Tilemap& GameManager::GetTilemap()
	{
		return m_Tilemap;
	}

	NetworkManager& GameManager::Network()
	{
		return m_Network;
	}

	SpellList& GameManager::Spells()
	{
		return m_Spells;
	}

	void GameManager::AddActiveFunction(Timer* function)
	{
		m_ActiveFunctions.push_back(function);
	}

	void GameManager::AddUIMenu(std::unique_ptr<UImenu>&& menu)
	{
		m_UImenus.push_back(std::move(menu));
	}

}