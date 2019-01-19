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
		: m_LocalCamera(nullptr), m_LocalPlayer(nullptr), m_Tilemap(layer, TILEMAP_WIDTH, TILEMAP_HEIGHT, TILE_WIDTH, TILE_HEIGHT), m_Prefabs(), m_Network(), m_Spells(), m_ActiveFunctions()
	{
	
	}

	void GameManager::Exit()
	{
		m_LocalCamera = nullptr;
		m_LocalPlayer = nullptr;
		Network().Exit([this]()
		{
			for (Timer* func : m_ActiveFunctions)
			{
				Time::RenderingTimeline().RemoveFunction(func);
			}
			SceneManager::SetCurrentSceneByName("Title");
		});		
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

	void GameManager::SetLocalPlayer(GameObject* player)
	{
		m_LocalPlayer = player;
		Network().SetPlayer(player);
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

}