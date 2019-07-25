#pragma once
#include "ChunkColumnData.h"

namespace Minecraft
{

	class ChunkRegion
	{
	private:
		int m_XChunks;
		int m_ZChunks;
		std::unique_ptr<ChunkColumnData*[]> m_Chunks;

	public:
		ChunkRegion();
		ChunkRegion(int xChunks, int zChunks);

		int GetTotalChunks() const;
		int GetXChunks() const;
		int GetZChunks() const;
		int GetYChunks() const;
		int GetWidthInBlocks() const;
		int GetHeightInBlocks() const;
		int GetDepthInBlocks() const;
		int GetXBlocksPerChunk() const;
		int GetZBlocksPerChunk() const;
		int GetYBlocksPerChunk() const;
		BlockId GetBlockId(int x, int y, int z) const;

		ChunkColumnData& GetChunkByXZIndex(int xIndex, int zIndex) const;
		ChunkColumnData& GetChunkByXZ(int x, int z) const;
		ChunkColumnData& GetChunkByIndex(int index) const;
		ChunkColumnData* GetChunkColumnPtr(int xIndex, int zIndex) const;
		void SetChunk(int x, int z, ChunkColumnData* chunk);

		void SetBlockId(int x, int y, int z, BlockId id) const;
		void GenerateDefaultTerrain(int stoneHeight, int dirtHeight, bool grass) const;

		std::vector<BlockFace> GetFaces(int xIndex, int zIndex) const;

	private:
		Vector2i GetXZIndex(int index) const;
		Vector2i GetRelativeXZFromChunk(int x, int z) const;
		Vector2i GetRelativeXZFromChunkIndices(int x, int z, int xChunkIndex, int zChunkIndex) const;

	};

}