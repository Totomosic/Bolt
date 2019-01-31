#pragma once
#include "bltpch.h"

#include "PrefabList.h"
#include "AnimationList.h"
#include "Map/TilemapManager.h"
#include "Networking/NetworkManager.h"
#include "Networking/NetworkIdentity.h"
#include "Spells/SpellList.h"
#include "PlayerManager.h"
#include "Entities/Characters/CharacterPrefabId.h"

namespace DND
{

	constexpr int TILE_WIDTH = 30;
	constexpr int TILE_HEIGHT = 30;

	struct PlayerCharacterInfo
	{
	public:
		id_t PrefabId;
	};

	struct GameStateObjects
	{
	public:
		AnimationList* Animations;
		PrefabList* Prefabs;
		NetworkManager* Network;
		ObjectFactory* Factory;
		TilemapManager* MapManager;
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
		TilemapManager m_TilemapManager;

		PrefabList m_Prefabs;
		AnimationList m_Animations;
		ObjectFactory m_Factory;
		NetworkManager m_Network;
		SpellList m_Spells;

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
		AnimationList& Animations();
		PrefabList& Prefabs();
		ObjectFactory& Factory();
		TilemapManager& MapManager();
		NetworkManager& Network();
		SpellList& Spells();

		GameObject* CreateObject(id_t mapId);
		GameObject* CreateCharacter(const CharacterInfo& character, id_t ownerNetworkId = GameObject::InvalidID);
		GameObject* CreateNetworkCharacter(const CharacterInfo& character, id_t ownerNetworkId = GameObject::InvalidID);
		void SetCurrentMap(id_t mapId);

		void AddActiveTimer(Timer* timer);

	private:
		void InitializeListeners();

	};

}