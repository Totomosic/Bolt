#include "mcpch.h"
#include "BlockDatabase.h"

namespace Minecraft
{

	TextureAtlas BlockDatabase::s_TextureAtlas;

	Block BlockDatabase::None;
	Block BlockDatabase::Air;
	Block BlockDatabase::Grass;
	Block BlockDatabase::Dirt;
	Block BlockDatabase::Stone;
	Block BlockDatabase::OakWood;
	Block BlockDatabase::OakLeaves;
	Block BlockDatabase::Sand;

	void BlockDatabase::Initialize(const TextureAtlas& textureAtlas)
	{
		s_TextureAtlas = textureAtlas;
		None = Block(BlockId::None, GetTextureCoordinates(BlockId::None), true, false);
		Air = Block(BlockId::Air, GetTextureCoordinates(BlockId::Air), false, false);
		Grass = Block(BlockId::Grass, GetTextureCoordinates(BlockId::Grass), true, true);
		Dirt = Block(BlockId::Dirt, GetTextureCoordinates(BlockId::Dirt), true, true);
		Stone = Block(BlockId::Stone, GetTextureCoordinates(BlockId::Stone), true, true);
		OakWood = Block(BlockId::OakWood, GetTextureCoordinates(BlockId::OakWood), true, true);
		OakLeaves = Block(BlockId::OakLeaves, GetTextureCoordinates(BlockId::OakLeaves), false, true);
		Sand = Block(BlockId::Sand, GetTextureCoordinates(BlockId::Sand), true, true);
	}

	const Block& BlockDatabase::GetBlock(BlockId id)
	{
		switch (id)
		{
		case BlockId::Air:
			return Air;
		case BlockId::Grass:
			return Grass;
		case BlockId::Dirt:
			return Dirt;
		case BlockId::Stone:
			return Stone;
		case BlockId::OakWood:
			return OakWood;
		case BlockId::OakLeaves:
			return OakLeaves;
		case BlockId::Sand:
			return Sand;
		}
		BLT_ASSERT(false, "Invalid block id");
		return None;
	}

	const TextureAtlas& BlockDatabase::GetTextureAtlas()
	{
		return s_TextureAtlas;
	}

	TextureCoordinateSet BlockDatabase::GetTextureCoordinates(BlockId id)
	{
		switch (id)
		{
		case BlockId::None:
			return {};
		case BlockId::Air:
			return {};
		case BlockId::Grass:
			return { s_TextureAtlas.GetImage(1), s_TextureAtlas.GetImage(0), s_TextureAtlas.GetImage(2) };
		case BlockId::Dirt:
			return { s_TextureAtlas.GetImage(2), s_TextureAtlas.GetImage(2), s_TextureAtlas.GetImage(2) };
		case BlockId::Stone:
			return { s_TextureAtlas.GetImage(3), s_TextureAtlas.GetImage(3), s_TextureAtlas.GetImage(3) };
		case BlockId::OakWood:
			return { s_TextureAtlas.GetImage(4), s_TextureAtlas.GetImage(5), s_TextureAtlas.GetImage(5) };
		case BlockId::OakLeaves:
			return { s_TextureAtlas.GetImage(6), s_TextureAtlas.GetImage(6), s_TextureAtlas.GetImage(6) };
		case BlockId::Sand:
			return { s_TextureAtlas.GetImage(7), s_TextureAtlas.GetImage(7), s_TextureAtlas.GetImage(7) };
		}
		BLT_ASSERT(false, "Invalid block id");
		return {};
	}

}