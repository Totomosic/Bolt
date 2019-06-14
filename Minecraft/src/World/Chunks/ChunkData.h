#pragma once
#include "BlockDatabase.h"

namespace Minecraft
{

	struct BlockFace
	{
	public:
		Vector3f TopLeft;
		Vector3f BottomLeft;
		Vector3f BottomRight;
		Vector3f TopRight;
		Vector3f Normal;
		TextureCoordinate Texture;
	};

	class ChunkData;

	struct ChunkContext
	{
	public:
		ChunkData* px = nullptr;
		ChunkData* nx = nullptr;
		ChunkData* py = nullptr;
		ChunkData* ny = nullptr;
		ChunkData* pz = nullptr;
		ChunkData* nz = nullptr;
	};

	class ChunkData
	{
	private:
		int m_Width; // x
		int m_Height; // z
		int m_Depth; // y
		std::unique_ptr<BlockId[]> m_Blocks;

	public:
		ChunkData();
		ChunkData(int width, int height, int depth);

		int GetWidth() const;
		int GetHeight() const;
		int GetDepth() const;
		BlockId GetBlockId(int x, int y, int z, const ChunkContext& context = ChunkContext()) const;

		void SetBlockId(int x, int y, int z, BlockId id) const;
		std::vector<BlockFace> GetFaces(const ChunkContext& context = ChunkContext()) const;

	private:
		BlockId& GetBlock(int x, int y, int z) const;

	};

}