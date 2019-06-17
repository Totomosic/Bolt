#pragma once
#include "ChunkData.h"

namespace Minecraft
{

	class ChunkColumnData;

	struct ChunkColumnContext
	{
	public:
		ChunkColumnData* px = nullptr;
		ChunkColumnData* nx = nullptr;
		ChunkColumnData* pz = nullptr;
		ChunkColumnData* nz = nullptr;
	};

	class ChunkColumnData
	{
	private:
		int m_NumChunks;
		std::unique_ptr<ChunkData[]> m_Chunks;

	public:
		ChunkColumnData();
		ChunkColumnData(int chunkHeight, int width, int height, int depth);

		int GetWidthInBlocks() const;
		int GetHeightInBlocks() const;
		int GetDepthInBlocks() const;
		int GetDepthOfChunkInBlocks() const;
		int GetNumberOfChunks() const;
		ChunkData& GetChunk(int index) const;
		ChunkData& GetChunkFromY(int y) const;
		BlockId GetBlockId(int x, int y, int z) const;
		void SetBlockId(int x, int y, int z, BlockId id) const;

		std::vector<BlockFace> GetFaces(const ChunkColumnContext& context = ChunkColumnContext()) const;

	private:
		int GetChunkIndexFromY(int y) const;
		ChunkData* GetChunkPtr(int index) const;

	};

}