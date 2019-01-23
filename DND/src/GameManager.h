#pragma once
#include "bltpch.h"

#include "PrefabList.h"
#include "Map/Tilemap.h"
#include "Networking/NetworkManager.h"
#include "Networking/NetworkIdentity.h"
#include "Spells/SpellList.h"

#include "UI/UImenu.h"

namespace DND
{

	constexpr int TILEMAP_WIDTH = 50;
	constexpr int TILEMAP_HEIGHT = 50;
	constexpr int TILE_WIDTH = 40;
	constexpr int TILE_HEIGHT = 40;

	struct PlayerCharacterInfo
	{
	public:
		id_t PrefabId;
	};

	struct GameStateObjects
	{
	public:
		NetworkManager* Network;
		ObjectFactory* Factory;
		Tilemap* Map;
		GameObject* LocalPlayer;
	};

	struct GameState
	{
	public:
		GameStateObjects Objects;
		Tile SelectedTile;
	};

	class GameManager
	{
	private:
		static GameManager* s_Instance;

	private:
		Camera* m_LocalCamera;
		GameObject* m_LocalPlayer;
		Tilemap m_Tilemap;

		PrefabList m_Prefabs;
		ObjectFactory m_Factory;
		NetworkManager m_Network;
		SpellList m_Spells;

		std::vector<std::unique_ptr<UImenu>> m_UImenus;

		std::vector<Timer*> m_ActiveFunctions;
		std::vector<Timer*> m_ActiveTimers;

	public:
		static GameManager& Get();
		static void Terminate();

		GameManager(Layer& layer);

		void Initialize();
		void Exit();
		void Exit(const std::function<void()>& callback);
		void Update();

		Camera* LocalCamera() const;
		GameObject* LocalPlayer() const;
		void SetLocalCamera(Camera* camera);

		GameStateObjects GetStateObjects();
		GameState GetGameState();
		Tile CurrentlySelectedTile() const;

		PrefabList& Prefabs();
		ObjectFactory& Factory();
		Tilemap& GetTilemap();
		NetworkManager& Network();
		SpellList& Spells();

		void AddActiveFunction(Timer* function);
		void AddActiveTimer(Timer* timer);
		void AddUIMenu(std::unique_ptr<UImenu>&& menu);

	};

}