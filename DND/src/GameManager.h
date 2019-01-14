#pragma once
#include "Bolt.h"
#include "Map/Tilemap.h"

namespace DND
{

	constexpr int TILEMAP_WIDTH = 50;
	constexpr int TILEMAP_HEIGHT = 50;
	constexpr int TILE_WIDTH = 50;
	constexpr int TILE_HEIGHT = 50;

	class GameManager
	{
	private:
		static GameManager* s_Instance;

	private:
		Camera* m_LocalCamera;
		GameObject* m_LocalPlayer;
		Tilemap m_Tilemap;

	public:
		static GameManager& Get();
		static void Terminate();

		GameManager(Layer& layer);

		Camera* LocalCamera() const;
		GameObject* LocalPlayer() const;
		void SetLocalCamera(Camera* camera);
		void SetLocalPlayer(GameObject* player);

		Tilemap& GetTilemap();

	};

}