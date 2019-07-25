#include "MapTileManager.h"
#include "GameManager.h"

namespace Blockstream
{

	MapTileManager::MapTileManager()
		: m_XTiles(0), m_YTiles(0), m_TileArray(nullptr), m_WaterStartX(0), m_WaterStartY(0)
	{
		
	}

	int MapTileManager::Width() const
	{
		return m_XTiles;
	}

	int MapTileManager::Height() const
	{
		return m_YTiles;
	}

	MapTile* MapTileManager::GetTile(int x, int y) const
	{
		if (x < 0 || y < 0 || x >= Width() || y >= Height())
		{
			return nullptr;
		}
		return m_TileArray[x + y * Width()];
	}

	std::pair<int, int> MapTileManager::GetCoordOfTile(MapTile* tile) const
	{
		for (int x = 0; x < Width(); x++)
		{
			for (int y = 0; y < Height(); y++)
			{
				if (GetTile(x, y) == tile)
				{
					return { x, y };
				}
			}
		}
		BLT_ASSERT(false, "Tile does not exist");
		return { 0, 0 };
	}

	void MapTileManager::SetStartTile(MapTile* start)
	{
		auto pair = GetCoordOfTile(start);
		m_WaterStartX = pair.first;
		m_WaterStartY = pair.second;
	}

	void MapTileManager::Init()
	{
		
	}

	id_t MapTileManager::CreatePrefab(ObjectFactory& factory, float width, float height, float depth)
	{
		Mesh mapTileMesh;
		mapTileMesh.Models.push_back({ ResourcePtr<Model>(new Model(CuboidFactory(width, depth, height)), true), Matrix4f::Identity(), { 0 } });
		mapTileMesh.Materials[0].Shader = Shader::LightingColor();
		ObjectPrefab mapTilePrefab;
		mapTilePrefab.Components().AddComponent<MeshRenderer>(mapTileMesh);
		mapTilePrefab.Components().AddComponent<MapTile>(0);
		m_TileWidth = width;
		m_TileHeight = height;
		return factory.AddPrefab(std::move(mapTilePrefab));
	}

	void MapTileManager::RecreateMap(ObjectFactory& factory, id_t prefabId, int xTiles, int yTiles, float waterStrength)
	{
		m_XTiles = xTiles;
		m_YTiles = yTiles;
		ObjectPrefab* prefab = factory.GetPrefab(prefabId);
		m_WaterStrength = waterStrength;
		if (m_TileArray != nullptr)
		{
			delete[] m_TileArray;
		}
		m_TileArray = new MapTile*[Width() * Height()];
		factory.CurrentLayer()->GameObjects().RemoveAllWithTag("MAP_TILE");

		for (int y = 0; y < Height(); y++)
		{
			float yPosition = -(Height() - 1) * m_TileHeight / 2.0f + y * m_TileHeight;
			for (int x = 0; x < Width(); x++)
			{
				float xPosition = -(Width() - 1) * m_TileWidth / 2.0f + x * m_TileWidth;
				GameObject* tile = factory.Instantiate(prefab, Transform({ xPosition, 0, yPosition }));
				tile->AddTag("MAP_TILE");
				m_TileArray[x + y * Width()] = &tile->Components().GetComponent<MapTile>();
			}
		}
	}

	void MapTileManager::RecolorMap()
	{
		for (int i = 0; i < Width() * Height(); i++)
		{
			m_TileArray[i]->SetWater(false);
		}
		TestWater(m_WaterStartX, m_WaterStartY, m_WaterStrength, 1000, m_WaterStrength);
	}

	void MapTileManager::TestWater(int x, int y, int waterStrength, int prevLevel, int maxWaterStrength) const
	{
		if (waterStrength > 0)
		{
			MapTile* tile = GetTile(x, y);
			if (tile != nullptr && tile->GetLevel() <= prevLevel)
			{
				tile->SetWater(true);
				int strength = waterStrength;
				if (tile->GetLevel() < prevLevel)
				{
					strength = maxWaterStrength;
				}
				for (int i = -1; i <= 1; i++)
				{
					if (i != 0)
					{
						TestWater(x + i, y, strength - 1, tile->GetLevel(), maxWaterStrength);
						TestWater(x, y + i, strength - 1, tile->GetLevel(), maxWaterStrength);
					}
				}
			}
		}
	}

}