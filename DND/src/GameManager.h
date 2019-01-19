#pragma once
#include "bltpch.h"

#include "PrefabList.h"
#include "Map/Tilemap.h"
#include "Networking/NetworkManager.h"
#include "Spells/SpellList.h"

namespace DND
{

	constexpr int TILEMAP_WIDTH = 50;
	constexpr int TILEMAP_HEIGHT = 50;
	constexpr int TILE_WIDTH = 40;
	constexpr int TILE_HEIGHT = 40;

	class GameManager
	{
	private:
		static GameManager* s_Instance;

	private:
		Camera* m_LocalCamera;
		GameObject* m_LocalPlayer;
		Tilemap m_Tilemap;

		PrefabList m_Prefabs;
		NetworkManager m_Network;
		SpellList m_Spells;

		std::vector<Timer*> m_ActiveFunctions;

	public:
		static GameManager& Get();
		static void Terminate();

		GameManager(Layer& layer);

		void Exit();

		Camera* LocalCamera() const;
		GameObject* LocalPlayer() const;
		void SetLocalCamera(Camera* camera);
		void SetLocalPlayer(GameObject* player);

		Tile CurrentlySelectedTile() const;

		PrefabList& Prefabs();
		Tilemap& GetTilemap();
		NetworkManager& Network();
		SpellList& Spells();
		
		void AddActiveFunction(Timer* function);

	};

}