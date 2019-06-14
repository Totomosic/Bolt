#pragma once
#include "Block.h"

namespace Minecraft
{

	class BlockDatabase
	{
	private:
		static TextureAtlas s_TextureAtlas;

	public:
		static Block None;
		static Block Air;
		static Block Grass;
		static Block Dirt;
		static Block Stone;
		static Block OakWood;
		static Block OakLeaves;
		static Block Sand;

	public:
		static void Initialize(const TextureAtlas& textureAtlas);
		static const Block& GetBlock(BlockId id);
		static const TextureAtlas& GetTextureAtlas();

	private:
		static TextureCoordinateSet GetTextureCoordinates(BlockId id);
	};

}