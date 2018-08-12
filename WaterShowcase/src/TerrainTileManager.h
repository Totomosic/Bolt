#pragma once
#include "BoltEngine.h"

using namespace Bolt;

namespace WaterShowcase
{

	class TerrainTileManager
	{
	public:
		static constexpr size_t DIRECTION_NORTH = 1;
		static constexpr size_t DIRECTION_NORTH_EAST = 2;
		static constexpr size_t DIRECTION_EAST = 3;
		static constexpr size_t DIRECTION_SOUTH_EAST = 4;
		static constexpr size_t DIRECTION_SOUTH = 5;
		static constexpr size_t DIRECTION_SOUTH_WEST = 6;
		static constexpr size_t DIRECTION_WEST = 7;
		static constexpr size_t DIRECTION_NORTH_WEST = 0;
		static constexpr size_t DIRECTION_CENTER = 8;

		static inline Vector3f DIRECTION_OFFSET_VECTORS[9] = { Vector3f(-1, 0, -1), Vector3f(0, 0, -1), Vector3f(1, 0, -1), Vector3f(1, 0, 0), Vector3f(1, 0, 1), Vector3f(0, 0, 1), Vector3f(-1, 0, 1), Vector3f(-1, 0, 0), Vector3f(0, 0, 0) };

	public:
		struct TerrainTile
		{
		public:
			Vector3f AbsolutePosition;
			Vector2f Size;
		};

	private:
		TerrainTile m_Tiles[9];
		float m_TileSize;
		int m_TileLOD;
		float m_Zoom;
		SimplexNoise m_NoiseGenerator;
		GameObject* m_TerrainObject;

	public:
		TerrainTileManager(SimplexNoise noiseGenerator, GameObject* terrainObject, float tileSize, int tileLOD, float zoom = 1.0f);

		void Update(const Vector3f& playerPosition);
		void MoveTiles(size_t direction);

	private:
		void ResetTiles();
		void UpdateTileModel(size_t index, const TerrainTile& tile) const;
		void UpdateAllTileModels() const;
		size_t IndexOfTile(size_t tileIndex) const;
		size_t OffsetToNextRowOfTile() const;

		size_t GetTileByPosition(float x, float z) const;
		bool IsInsideTile(const TerrainTile& tile, float x, float z) const;

	};

}