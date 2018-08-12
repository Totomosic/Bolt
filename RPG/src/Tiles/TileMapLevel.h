#pragma once
#include "MapTile.h"
#include "..\Entities\NPCs\NPCcharacter.h"

namespace RPG
{

	class PlayerWizard;

	class TileMapLevel
	{
	private:
		int m_Width;
		int m_Height;
		Vector2i m_TileSize;
		std::vector<std::unique_ptr<MapTile>> m_Tiles; // 2D Grid Origin at bottom left
		std::vector<std::unique_ptr<NPCcharacter>> m_NPCs;
		id_t m_Layer;

	public:
		TileMapLevel(int width, int height, Vector2i tileSize, id_t layerId = 0);
		TileMapLevel(const Image& level, Vector2i tileSize, id_t layerId = 0);

		int Width() const;
		int Height() const;
		const Vector2i& TileSize() const;
		const std::vector<std::unique_ptr<MapTile>>& Tiles() const;
		const MapTile& GetTile(int x, int y) const;
		MapTile& GetTile(int x, int y);
		NPCcharacter* GetNPC(int index) const;

		NPCcharacter* AddNPC(std::unique_ptr<NPCcharacter>&& npc, bool create = false);
		void RemoveNPC(int index);
		void RemoveNPC(NPCcharacter* character);

		MapTile& CreateTile(int x, int y, TileType type);
		void Update(const PlayerWizard* player);

		void Create(int x, int y, int width, int height);
		void Create();
		void Destroy(int x, int y, int width, int height);
		void Destroy();
		void CreateNPCs();
		void DestroyNPCs();

	};

}