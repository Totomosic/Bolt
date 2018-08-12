#include "TerrainTileManager.h"

namespace WaterShowcase
{

	TerrainTileManager::TerrainTileManager(SimplexNoise noiseGenerator, GameObject* terrainObject, float tileSize, int tileLOD, float zoom)
		: m_Tiles(), m_TileSize(tileSize), m_TileLOD(tileLOD), m_Zoom(zoom), m_NoiseGenerator(noiseGenerator), m_TerrainObject(terrainObject)
	{
		MeshRenderer& mesh = m_TerrainObject->Components().GetComponent<MeshRenderer>();
		mesh.Mesh.Models.push_back({ new Model(GridFactory(m_TileSize * 3, m_TileSize * 3, m_TileLOD * 3, m_TileLOD * 3)), Matrix4f::Identity(), { 0 } });
		ResetTiles();
	}

	void TerrainTileManager::Update(const Vector3f& playerPosition)
	{
		Vector3f centerPosition = m_TerrainObject->transform().Position();
		size_t tile = GetTileByPosition(playerPosition.x, playerPosition.z);
		if (tile != DIRECTION_CENTER)
		{
			MoveTiles(tile);
		}
	}

	void TerrainTileManager::ResetTiles()
	{
		Vector3f centerPosition = m_TerrainObject->transform().Position();
		for (int i = 0; i <= DIRECTION_CENTER; i++)
		{
			m_Tiles[i] = { centerPosition + DIRECTION_OFFSET_VECTORS[i] * m_TileSize, Vector2f(m_TileSize, m_TileSize) };
		}
		UpdateAllTileModels();
	}

	void TerrainTileManager::UpdateTileModel(size_t index, const TerrainTileManager::TerrainTile& tile) const
	{
		MeshRenderer& meshRenderer = m_TerrainObject->Components().GetComponent<MeshRenderer>();
		Mesh& mesh = meshRenderer.Mesh;
		const Model* model = mesh.Models[0].Model;

		float minX = (tile.AbsolutePosition.x - tile.Size.x / 2.0f) / m_TileSize * m_Zoom;
		float maxX = (tile.AbsolutePosition.x + tile.Size.x / 2.0f) / m_TileSize * m_Zoom;
		float minZ = (tile.AbsolutePosition.z - tile.Size.y / 2.0f) / m_TileSize * m_Zoom;
		float maxZ = (tile.AbsolutePosition.z + tile.Size.y / 2.0f) / m_TileSize * m_Zoom;

		VertexIterator iterator = model->Data().Vertices->Begin();
		iterator += IndexOfTile(index);
		for (int z = 0; z < m_TileLOD; z++)
		{
			for (int x = 0; x < m_TileLOD; x++)
			{
				float xCoord = Map<float>(x, 0, m_TileLOD - 1, minX, maxX);
				float zCoord = Map<float>(z, 0, m_TileLOD - 1, minZ, maxZ);
				float height = m_NoiseGenerator.Generate(4, xCoord, zCoord) * 40;
				iterator[0].Read<Vector3f>().y = height;
				iterator++;
			}
			iterator += OffsetToNextRowOfTile();
		}
	}

	void TerrainTileManager::UpdateAllTileModels() const
	{
		MeshRenderer& meshRenderer = m_TerrainObject->Components().GetComponent<MeshRenderer>();
		Mesh& mesh = meshRenderer.Mesh;
		const Model* model = mesh.Models[0].Model;
		Vector3f centerPosition = m_Tiles[DIRECTION_CENTER].AbsolutePosition;

		float minX = (centerPosition.x - m_TileSize * 3 / 2.0f) / m_TileSize * m_Zoom;
		float maxX = (centerPosition.x + m_TileSize * 3 / 2.0f) / m_TileSize * m_Zoom;
		float minZ = (centerPosition.z - m_TileSize * 3 / 2.0f) / m_TileSize * m_Zoom;
		float maxZ = (centerPosition.z + m_TileSize * 3 / 2.0f) / m_TileSize * m_Zoom;

		VertexIterator iterator = model->Data().Vertices->Begin();
		for (int z = 0; z < m_TileLOD * 3; z++)
		{
			for (int x = 0; x < m_TileLOD * 3; x++)
			{
				float xCoord = Map<float>(x, 0, m_TileLOD * 3 - 1, minX, maxX);
				float zCoord = Map<float>(z, 0, m_TileLOD * 3 - 1, minZ, maxZ);
				float height = m_NoiseGenerator.Generate(4, xCoord, zCoord) * 40;
				iterator[0].Read<Vector3f>().y = height;
				iterator++;
			}
		}		
	}

	size_t TerrainTileManager::IndexOfTile(size_t tileIndex) const
	{	
		switch (tileIndex)
		{
		case DIRECTION_NORTH_WEST:
			return 0;
		case DIRECTION_NORTH:
			return m_TileLOD;
		case DIRECTION_NORTH_EAST:
			return m_TileLOD * 2;
		case DIRECTION_WEST:
			return m_TileLOD * 3 * m_TileLOD;
		case DIRECTION_CENTER:
			return m_TileLOD * 3 * m_TileLOD + m_TileLOD;
		case DIRECTION_EAST:
			return m_TileLOD * 3 * m_TileLOD + m_TileLOD * 2;
		case DIRECTION_SOUTH_WEST:
			return m_TileLOD * 3 * m_TileLOD * 2;
		case DIRECTION_SOUTH:
			return m_TileLOD * 3 * m_TileLOD * 2 + m_TileLOD;
		case DIRECTION_SOUTH_EAST:
			return m_TileLOD * 3 * m_TileLOD * 2 + m_TileLOD * 2;
		}
		return 0;
	}

	size_t TerrainTileManager::OffsetToNextRowOfTile() const
	{
		return m_TileLOD * 2;
	}

	void TerrainTileManager::MoveTiles(size_t direction)
	{
		for (int i = 0; i < 9; i++)
		{
			m_Tiles[i].AbsolutePosition += DIRECTION_OFFSET_VECTORS[direction] * m_TileSize;
		}
		UpdateAllTileModels();
		m_TerrainObject->Components().GetComponent<MeshRenderer>().Mesh.Models[0].Transform *= Matrix4f::Translation(DIRECTION_OFFSET_VECTORS[direction] * m_TileSize);
	}

	size_t TerrainTileManager::GetTileByPosition(float x, float z) const
	{
		for (int i = DIRECTION_CENTER; i >= 0; i--)
		{
			if (IsInsideTile(m_Tiles[i], x, z))
			{
				return (size_t)i;
			}
		}
	}

	bool TerrainTileManager::IsInsideTile(const TerrainTileManager::TerrainTile& tile, float x, float z) const
	{
		float tileX = tile.AbsolutePosition.x;
		float tileZ = tile.AbsolutePosition.z;
		float tileWidth = tile.Size.x / 2.0f;
		float tileDepth = tile.Size.y / 2.0f;
		return (x >= tileX - tileWidth && x <= tileX + tileWidth) && (z >= tileZ - tileDepth && z <= tileZ + tileDepth);
	}

}