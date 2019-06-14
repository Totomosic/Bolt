#pragma once
#include "ChunkData.h"

namespace Minecraft
{

	class ChunkColumnData
	{
	private:
		int m_ChunkHeigt;
		std::unique_ptr<ChunkData[]> m_Chunks;

	public:
		ChunkColumnData();
		ChunkColumnData(int chunkHeight);

		int GetChunkHeight() const;
		ChunkData& GetChunk(int index) const;

		std::vector<BlockFace> GetFaces() const;

	};

}