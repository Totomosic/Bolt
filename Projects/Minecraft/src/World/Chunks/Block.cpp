#include "mcpch.h"
#include "Block.h"

namespace Minecraft
{

	Block::Block() : Block(BlockId::None, {}, false, false)
	{
	
	}

	Block::Block(BlockId id, const TextureCoordinateSet& texture, bool isOpaque, bool isCollidable)
		: m_BlockId(id), m_Texture(texture), m_IsOpaque(isOpaque), m_IsCollidable(isCollidable)
	{
	
	}

	BlockId Block::GetId() const
	{
		return m_BlockId;
	}

	const TextureCoordinateSet& Block::GetTexture() const
	{
		return m_Texture;
	}

	bool Block::IsOpaque() const
	{
		return m_IsOpaque;
	}

	bool Block::IsCollidable() const
	{
		return m_IsCollidable;
	}

}