#include "mcpch.h"
#include "ChunkData.h"

namespace Minecraft
{

	ChunkData::ChunkData() : ChunkData(0, 0, 0)
	{
	
	}

	ChunkData::ChunkData(int width, int height, int depth)
		: m_Width(width), m_Height(height), m_Depth(depth), m_Blocks(nullptr)
	{
		if (m_Width > 0 && m_Height > 0 && m_Depth > 0)
		{
			m_Blocks = std::make_unique<BlockId[]>((uint64_t)m_Width * (uint64_t)m_Height * (uint64_t)m_Depth);
			for (int i = 0; i < GetWidth() * GetHeight() * GetDepth(); i++)
			{
				m_Blocks[i] = BlockId::Air;
			}
		}
	}

	int ChunkData::GetWidth() const
	{
		return m_Width;
	}

	int ChunkData::GetHeight() const
	{
		return m_Height;
	}

	int ChunkData::GetDepth() const
	{
		return m_Depth;
	}

	BlockId ChunkData::GetBlockId(int x, int y, int z, const ChunkContext& context) const
	{
		if (x < 0)
		{
			if (context.nx)
			{
				return context.nx->GetBlockId(context.nx->GetWidth() + x, y, z);
			}
			return BlockId::Air;
		}
		if (y < 0)
		{
			if (context.ny)
			{
				return context.ny->GetBlockId(x, context.ny->GetDepth() + y, z);
			}
			return BlockId::Air;
		}
		if (z < 0)
		{
			if (context.nz)
			{
				return context.nz->GetBlockId(x, y, context.nz->GetHeight() + z);
			}
			return BlockId::Air;
		}
		if (x >= GetWidth())
		{
			if (context.px)
			{
				return context.px->GetBlockId(x - GetWidth(), y, z);
			}
			return BlockId::Air;
		}
		if (y >= GetDepth())
		{
			if (context.py)
			{
				return context.py->GetBlockId(x, y - GetDepth(), z);
			}
			return BlockId::Air;
		}
		if (z >= GetHeight())
		{
			if (context.pz)
			{
				return context.pz->GetBlockId(x, y, z - GetHeight());
			}
			return BlockId::Air;
		}
		return GetBlock(x, y, z);
	}

	void ChunkData::SetBlockId(int x, int y, int z, BlockId id) const
	{
		GetBlock(x, y, z) = id;
	}

	std::vector<BlockFace> ChunkData::GetFaces(const ChunkContext& context) const
	{
		std::vector<BlockFace> faces;
		for (int i = 0; i < GetWidth(); i++)
		{
			for (int j = 0; j < GetDepth(); j++)
			{
				for (int k = 0; k < GetHeight(); k++)
				{
					BlockId id = GetBlockId(i, j, k);
					if (id != BlockId::Air && id != BlockId::None)
					{
						float xPos = i * BLOCK_SIZE;
						float yPos = j * BLOCK_SIZE;
						float zPos = k * BLOCK_SIZE;
						BlockId top = GetBlockId(i, j + 1, k, context);
						BlockId bot = GetBlockId(i, j - 1, k, context);
						BlockId px = GetBlockId(i + 1, j, k, context);
						BlockId nx = GetBlockId(i - 1, j, k, context);
						BlockId pz = GetBlockId(i, j, k + 1, context);
						BlockId nz = GetBlockId(i, j, k - 1, context);
						if (top == BlockId::Air)
						{
							BlockFace f;
							f.Normal = Vector3f(0, 1, 0);
							f.Texture = BlockDatabase::GetBlock(id).GetTexture().Top;
							f.TopLeft = Vector3f(xPos, yPos + BLOCK_SIZE, zPos);
							f.BottomLeft = Vector3f(xPos, yPos + BLOCK_SIZE, zPos + BLOCK_SIZE);
							f.BottomRight = Vector3f(xPos + BLOCK_SIZE, yPos + BLOCK_SIZE, zPos + BLOCK_SIZE);
							f.TopRight = Vector3f(xPos + BLOCK_SIZE, yPos + BLOCK_SIZE, zPos);
							faces.push_back(f);
						}
						if (bot == BlockId::Air)
						{
							BlockFace f;
							f.Normal = Vector3f(0, -1, 0);
							f.Texture = BlockDatabase::GetBlock(id).GetTexture().Bottom;
							f.TopLeft = Vector3f(xPos, yPos, zPos + BLOCK_SIZE);
							f.BottomLeft = Vector3f(xPos, yPos, zPos);
							f.BottomRight = Vector3f(xPos + BLOCK_SIZE, yPos, zPos);
							f.TopRight = Vector3f(xPos + BLOCK_SIZE, yPos, zPos + BLOCK_SIZE);
							faces.push_back(f);
						}
						if (px == BlockId::Air)
						{
							BlockFace f;
							f.Normal = Vector3f(1, 0, 0);
							f.Texture = BlockDatabase::GetBlock(id).GetTexture().Side;
							f.TopLeft = Vector3f(xPos + BLOCK_SIZE, yPos + BLOCK_SIZE, zPos + BLOCK_SIZE);
							f.BottomLeft = Vector3f(xPos + BLOCK_SIZE, yPos, zPos + BLOCK_SIZE);
							f.BottomRight = Vector3f(xPos + BLOCK_SIZE, yPos, zPos);
							f.TopRight = Vector3f(xPos + BLOCK_SIZE, yPos + BLOCK_SIZE, zPos);
							faces.push_back(f);
						}
						if (nx == BlockId::Air)
						{
							BlockFace f;
							f.Normal = Vector3f(-1, 0, 0);
							f.Texture = BlockDatabase::GetBlock(id).GetTexture().Side;
							f.TopLeft = Vector3f(xPos, yPos + BLOCK_SIZE, zPos);
							f.BottomLeft = Vector3f(xPos, yPos, zPos);
							f.BottomRight = Vector3f(xPos, yPos, zPos + BLOCK_SIZE);
							f.TopRight = Vector3f(xPos, yPos + BLOCK_SIZE, zPos + BLOCK_SIZE);
							faces.push_back(f);
						}
						if (pz == BlockId::Air)
						{
							BlockFace f;
							f.Normal = Vector3f(0, 0, 1);
							f.Texture = BlockDatabase::GetBlock(id).GetTexture().Side;
							f.TopLeft = Vector3f(xPos, yPos + BLOCK_SIZE, zPos + BLOCK_SIZE);
							f.BottomLeft = Vector3f(xPos, yPos, zPos + BLOCK_SIZE);
							f.BottomRight = Vector3f(xPos + BLOCK_SIZE, yPos, zPos + BLOCK_SIZE);
							f.TopRight = Vector3f(xPos + BLOCK_SIZE, yPos + BLOCK_SIZE, zPos + BLOCK_SIZE);
							faces.push_back(f);
						}
						if (nz == BlockId::Air)
						{
							BlockFace f;
							f.Normal = Vector3f(0, 0, -1);
							f.Texture = BlockDatabase::GetBlock(id).GetTexture().Side;
							f.TopLeft = Vector3f(xPos + BLOCK_SIZE, yPos + BLOCK_SIZE, zPos);
							f.BottomLeft = Vector3f(xPos + BLOCK_SIZE, yPos, zPos);
							f.BottomRight = Vector3f(xPos, yPos, zPos);
							f.TopRight = Vector3f(xPos, yPos + BLOCK_SIZE, zPos);
							faces.push_back(f);
						}
					}
				}
			}
		}
		return faces;
	}

	BlockId& ChunkData::GetBlock(int x, int y, int z) const
	{
		BLT_ASSERT(x >= 0 && x < GetWidth() && y >= 0 && y < GetDepth() && z >= 0 && z < GetHeight(), "Invalid block");
		return m_Blocks[y * GetWidth() * GetHeight() + z * GetWidth() + x];
	}

}