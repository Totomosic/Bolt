#include "mcpch.h"
#include "ChunkRegion.h"

namespace Minecraft
{

	ChunkRegion::ChunkRegion() : ChunkRegion(0, 0)
	{
	
	}

	ChunkRegion::ChunkRegion(int xChunks, int zChunks)
		: m_XChunks(xChunks), m_ZChunks(zChunks), m_Chunks(nullptr)
	{
		if (m_XChunks > 0 && m_ZChunks > 0)
		{
			m_Chunks = std::make_unique<ChunkColumnData*[]>((uint64_t)m_XChunks * (uint64_t)m_ZChunks);
			for (int i = 0; i < GetTotalChunks(); i++)
			{
				m_Chunks[i] = nullptr;
			}
		}
	}

	int ChunkRegion::GetTotalChunks() const
	{
		return m_XChunks * m_ZChunks;
	}

	int ChunkRegion::GetXChunks() const
	{
		return m_XChunks;
	}

	int ChunkRegion::GetZChunks() const
	{
		return m_ZChunks;
	}

	int ChunkRegion::GetYChunks() const
	{
		BLT_ASSERT(GetTotalChunks() > 0, "no chunks");
		return GetChunkByIndex(0).GetNumberOfChunks();
	}

	int ChunkRegion::GetWidthInBlocks() const
	{
		BLT_ASSERT(GetTotalChunks() > 0, "no chunks");
		return GetChunkByIndex(0).GetWidthInBlocks() * GetXChunks();
	}

	int ChunkRegion::GetHeightInBlocks() const
	{
		BLT_ASSERT(GetTotalChunks() > 0, "no chunks");
		return GetChunkByIndex(0).GetHeightInBlocks() * GetZChunks();
	}

	int ChunkRegion::GetDepthInBlocks() const
	{
		BLT_ASSERT(GetTotalChunks() > 0, "no chunks");
		return GetChunkByIndex(0).GetDepthInBlocks();
	}

	int ChunkRegion::GetXBlocksPerChunk() const
	{
		BLT_ASSERT(GetTotalChunks() > 0, "no chunks");
		return GetChunkByIndex(0).GetWidthInBlocks();
	}

	int ChunkRegion::GetZBlocksPerChunk() const
	{
		BLT_ASSERT(GetTotalChunks() > 0, "no chunks");
		return GetChunkByIndex(0).GetHeightInBlocks();
	}

	int ChunkRegion::GetYBlocksPerChunk() const
	{
		BLT_ASSERT(GetTotalChunks() > 0, "no chunks");
		return GetChunkByIndex(0).GetDepthOfChunkInBlocks();
	}

	BlockId ChunkRegion::GetBlockId(int x, int y, int z) const
	{
		BLT_ASSERT(x >= 0 && x < GetWidthInBlocks() && y >= 0 && y < GetDepthInBlocks() && z >= 0 && z < GetHeightInBlocks(), "invalid xyz");
		ChunkColumnData& chunk = GetChunkByXZ(x, z);
		Vector2i relXZ = GetRelativeXZFromChunk(x, z);
		return chunk.GetBlockId(relXZ.x, y, relXZ.y);
	}

	ChunkColumnData& ChunkRegion::GetChunkByXZIndex(int xIndex, int zIndex) const
	{
		BLT_ASSERT(xIndex >= 0 && xIndex < GetXChunks() && zIndex >= 0 && zIndex < GetZChunks(), "invalid xIndex zIndex");
		return GetChunkByIndex(xIndex + zIndex * GetXChunks());
	}

	ChunkColumnData& ChunkRegion::GetChunkByXZ(int x, int z) const
	{
		int xIndex = x / GetXBlocksPerChunk();
		int zIndex = z / GetZBlocksPerChunk();
		return GetChunkByXZIndex(xIndex, zIndex);
	}

	ChunkColumnData& ChunkRegion::GetChunkByIndex(int index) const
	{
		BLT_ASSERT(index >= 0 && index < GetTotalChunks(), "invalid index");
		return *m_Chunks[index];
	}

	ChunkColumnData* ChunkRegion::GetChunkColumnPtr(int xIndex, int zIndex) const
	{
		if (xIndex < 0 || xIndex >= GetXChunks() || zIndex < 0 || zIndex >= GetZChunks())
		{
			return nullptr;
		}
		return &GetChunkByXZIndex(xIndex, zIndex);
	}

	void ChunkRegion::SetChunk(int x, int z, ChunkColumnData* chunk)
	{
		BLT_ASSERT(x >= 0 && x < GetXChunks() && z >= 0 && z < GetZChunks(), "invalid xz");
		m_Chunks[x + z * GetXChunks()] = chunk;
	}

	void ChunkRegion::SetBlockId(int x, int y, int z, BlockId id) const
	{
		BLT_ASSERT(x >= 0 && x < GetWidthInBlocks() && y >= 0 && y < GetDepthInBlocks() && z >= 0 && z < GetHeightInBlocks(), "invalid xyz");
		ChunkColumnData& chunk = GetChunkByXZ(x, z);
		Vector2i relXZ = GetRelativeXZFromChunk(x, z);
		return chunk.SetBlockId(relXZ.x, y, relXZ.y, id);
	}

	void ChunkRegion::GenerateDefaultTerrain(int stoneHeight, int dirtHeight, bool grass) const
	{
		for (int x = 0; x < GetWidthInBlocks(); x++)
		{
			for (int z = 0; z < GetHeightInBlocks(); z++)
			{
				for (int y = 0; y < GetDepthInBlocks(); y++)
				{
					if (y < stoneHeight)
					{
						SetBlockId(x, y, z, BlockId::Stone);
					}
					else if (y < dirtHeight)
					{
						SetBlockId(x, y, z, BlockId::Dirt);
					}
					else if (grass && y < dirtHeight + 1)
					{
						SetBlockId(x, y, z, BlockId::Grass);
					}
					else
					{
						SetBlockId(x, y, z, BlockId::Air);
					}
				}
			}
		}
	}

	std::vector<BlockFace> ChunkRegion::GetFaces(int x, int z) const
	{
		ChunkColumnContext context;
		context.px = GetChunkColumnPtr(x + 1, z);
		context.nx = GetChunkColumnPtr(x - 1, z);
		context.pz = GetChunkColumnPtr(x, z + 1);
		context.nz = GetChunkColumnPtr(x, z - 1);
		ChunkColumnData& chunk = GetChunkByXZIndex(x, z);
		auto faces = chunk.GetFaces(context);
		return faces;
	}

	Vector2i ChunkRegion::GetXZIndex(int index) const
	{
		return { index % GetXChunks(), index / GetXChunks() };
	}

	Vector2i ChunkRegion::GetRelativeXZFromChunk(int x, int z) const
	{
		int xIndex = x / GetXBlocksPerChunk();
		int zIndex = z / GetZBlocksPerChunk();
		return GetRelativeXZFromChunkIndices(x, z, xIndex, zIndex);
	}

	Vector2i ChunkRegion::GetRelativeXZFromChunkIndices(int x, int z, int xChunkIndex, int zChunkIndex) const
	{
		return { x - xChunkIndex * GetXBlocksPerChunk(), z - zChunkIndex * GetZBlocksPerChunk() };
	}

}