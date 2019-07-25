#pragma once
#include "BoltInclude.h"
#include "MapTile.h"

namespace Blockstream
{

	class MapTileManager
	{
	private:
		int m_XTiles;
		int m_YTiles;
		float m_TileWidth;
		float m_TileHeight;
		MapTile** m_TileArray;
		float m_WaterStrength;

		int m_WaterStartX;
		int m_WaterStartY;

	public:
		MapTileManager();

		int Width() const;
		int Height() const;
		MapTile* GetTile(int x, int y) const;
		std::pair<int, int> GetCoordOfTile(MapTile* tile) const;

		void SetStartTile(MapTile* tile);

		void Init();
		id_t CreatePrefab(ObjectFactory& factory, float width, float height, float depth);
		void RecreateMap(ObjectFactory& factory, id_t prefab, int xTiles, int yTiles, float waterStrength);
		void RecolorMap();
		void TestWater(int x, int y, int waterStrength, int prevLevel, int maxWaterStrength) const;

	};

}