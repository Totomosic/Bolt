#pragma once
#include "bltpch.h"

#include "PrefabList.h"
#include "Map/Tilemap.h"
#include "Networking/NetworkManager.h"
#include "Networking/NetworkIdentity.h"
#include "Spells/SpellList.h"
#include "PlayerManager.h"

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
		PlayerManager* Players;
	};

	struct GameState
	{
	public:
		GameStateObjects Objects;
		Tile SelectedTile;
	};

	class GameManager
	{
	public:
		using LoadGameCallback = std::function<void(const WelcomePacket&, PlayerCharacterInfo)>;

	private:
		static GameManager* s_Instance;

	private:
		Camera* m_LocalCamera;
		PlayerManager m_Players;
		Tilemap m_Tilemap;

		PrefabList m_Prefabs;
		ObjectFactory m_Factory;
		NetworkManager m_Network;
		SpellList m_Spells;

		std::vector<Timer*> m_ActiveFunctions;
		std::vector<Timer*> m_ActiveTimers;

	public:
		static GameManager& Get();
		static void Terminate();

	private:
		GameManager(Layer& layer);

	public:
		void Host(PlayerCharacterInfo player, LoadGameCallback callback);
		void Join(id_t connectionId, PlayerCharacterInfo player, LoadGameCallback callback);

		void Initialize();
		void Exit();
		void Exit(const std::function<void()>& callback);
		void Update();

		Camera* LocalCamera() const;
		void SetLocalCamera(Camera* camera);

		GameStateObjects GetStateObjects();
		GameState GetGameState();
		Tile CurrentlySelectedTile() const;

		PlayerManager& Players();
		PrefabList& Prefabs();
		ObjectFactory& Factory();
		Tilemap& GetTilemap();
		NetworkManager& Network();
		SpellList& Spells();

		void AddActiveFunction(Timer* function);
		void AddActiveTimer(Timer* timer);

	private:
		void InitializeListeners();

	};

}