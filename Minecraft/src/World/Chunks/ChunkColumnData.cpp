#include "mcpch.h"
#include "ChunkColumnData.h"

namespace Minecraft
{

	ChunkColumnData::ChunkColumnData() : ChunkColumnData(0, 0, 0, 0)
	{
	
	}

	ChunkColumnData::ChunkColumnData(int numChunks, int width, int height, int depth)
		: m_NumChunks(numChunks), m_Chunks(nullptr)
	{
		if (m_NumChunks > 0)
		{
			m_Chunks = std::make_unique<ChunkData[]>(m_NumChunks);
			for (int i = 0; i < m_NumChunks; i++)
			{
				m_Chunks[i] = ChunkData(width, height, depth);
			}
		}
	}

	int ChunkColumnData::GetWidthInBlocks() const
	{
		BLT_ASSERT(m_NumChunks > 0, "no chunks");
		return m_Chunks[0].GetWidth();
	}

	int ChunkColumnData::GetHeightInBlocks() const
	{
		BLT_ASSERT(m_NumChunks > 0, "no chunks");
		return m_Chunks[0].GetHeight();
	}

	int ChunkColumnData::GetDepthInBlocks() const
	{
		return GetDepthOfChunkInBlocks() * m_NumChunks;
	}

	int ChunkColumnData::GetDepthOfChunkInBlocks() const
	{
		BLT_ASSERT(m_NumChunks > 0, "no chunks");
		return m_Chunks[0].GetDepth();
	}

	int ChunkColumnData::GetNumberOfChunks() const
	{
		return m_NumChunks;
	}

	ChunkData& ChunkColumnData::GetChunk(int index) const
	{
		BLT_ASSERT(index >= 0 && index < GetNumberOfChunks(), "Invalid chunk index");
		return m_Chunks[index];
	}

	ChunkData& ChunkColumnData::GetChunkFromY(int y) const
	{
		return GetChunk(GetChunkIndexFromY(y));
	}

	BlockId ChunkColumnData::GetBlockId(int x, int y, int z) const
	{
		int index = GetChunkIndexFromY(y);
		ChunkData& chunk = GetChunkFromY(y);
		y -= index * chunk.GetDepth();
		return chunk.GetBlockId(x, y, z);
	}

	void ChunkColumnData::SetBlockId(int x, int y, int z, BlockId id) const
	{
		int index = GetChunkIndexFromY(y);
		ChunkData& chunk = GetChunkFromY(y);
		y -= index * chunk.GetDepth();
		return chunk.SetBlockId(x, y, z, id);
	}

	std::vector<BlockFace> ChunkColumnData::GetFaces(const ChunkColumnContext& context) const
	{
		std::vector<BlockFace> result;
		for (int i = 0; i < m_NumChunks; i++)
		{
			ChunkContext chunkContext;
			chunkContext.ny = GetChunkPtr(i - 1);
			chunkContext.py = GetChunkPtr(i + 1);
			if (context.px)
			{
				chunkContext.px = context.px->GetChunkPtr(i);
			}
			if (context.nx)
			{
				chunkContext.nx = context.nx->GetChunkPtr(i);
			}
			if (context.pz)
			{
				chunkContext.pz = context.pz->GetChunkPtr(i);
			}
			if (context.nz)
			{
				chunkContext.nz = context.nz->GetChunkPtr(i);
			}
			auto& chunk = GetChunk(i);
			auto faces = chunk.GetFaces(chunkContext);
			for (auto& face : faces)
			{
				face.TopLeft.y += i * chunk.GetDepth() * BLOCK_SIZE;
				face.BottomLeft.y += i * chunk.GetDepth() * BLOCK_SIZE;
				face.BottomRight.y += i * chunk.GetDepth() * BLOCK_SIZE;
				face.TopRight.y += i * chunk.GetDepth() * BLOCK_SIZE;
			}
			result = ConcatVectors(result, faces);
		}
		return result;
	}

	int ChunkColumnData::GetChunkIndexFromY(int y) const
	{
		BLT_ASSERT(y >= 0.0f && y < GetDepthInBlocks(), "invalid y");
		return (int)(y / GetDepthOfChunkInBlocks());
	}

	ChunkData* ChunkColumnData::GetChunkPtr(int index) const
	{
		if (index < 0 || index >= GetNumberOfChunks())
		{
			return nullptr;
		}
		return &m_Chunks[index];
	}

}