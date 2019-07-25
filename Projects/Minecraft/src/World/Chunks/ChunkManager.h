#pragma once
#include "ChunkRegion.h"

namespace Minecraft
{

	class ChunkManager
	{
	public:
		struct ChunkLocation
		{
		public:
			Vector2i ChunkPosition;
			std::unique_ptr<ChunkColumnData> Chunk;
		};

		struct ChunkObject
		{
		public:
			ResourcePtr<Model> ModelPtr;
			GameObject* Object;
		};

	private:
		int m_VisibleXChunks;
		int m_VisibleZChunks;
		ChunkRegion m_VisibleRegion;
		std::vector<ChunkLocation> m_LoadedChunks;
		std::unique_ptr<ChunkObject[]> m_ChunkObjects;

	public:
		ChunkManager(ObjectFactory& factory, const ResourcePtr<Texture2D>& atlas, int visibleXChunks, int visibleZChunks);

		int GetVisibleXChunks() const;
		int GetVisibleZChunks() const;
		const ChunkRegion& GetChunkRegion() const;
		ChunkRegion& GetChunkRegion();

		void BuildChunk(int x, int z) const;
		void BuildAllChunks() const;
	};

}