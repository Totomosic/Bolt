#pragma once
#include "Textures/TextureAtlas.h"
#include "BlockTypes.h"

namespace Minecraft
{

	extern float BLOCK_SIZE;

	struct TextureCoordinateSet
	{
	public:
		TextureCoordinate Side;
		TextureCoordinate Top;
		TextureCoordinate Bottom;
	};

	class Block
	{
	private:
		BlockId m_BlockId;
		TextureCoordinateSet m_Texture;
		bool m_IsOpaque;
		bool m_IsCollidable;

	public:
		Block();
		Block(BlockId id, const TextureCoordinateSet& texture, bool isOpaque = true, bool isCollidable = true);

		BlockId GetId() const;
		const TextureCoordinateSet& GetTexture() const;
		bool IsOpaque() const;
		bool IsCollidable() const;

	};

}