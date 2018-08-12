#pragma once
#include "TextureAnimator.h"
#include "..\..\Textures\__Textures__.h"

namespace Bolt
{

	struct BLT_API TextureGroup
	{
	public:
		std::vector<const Texture*> Textures;
		std::unordered_map<int, std::unique_ptr<TextureAnimator>> Animators;

	public:
		TextureGroup() = default;
		TextureGroup(const TextureGroup& other);
		TextureGroup& operator=(const TextureGroup& other);
		TextureGroup(TextureGroup&& other) = default;
		TextureGroup& operator=(TextureGroup&& other) = default;
		~TextureGroup() = default;

		bool operator==(const TextureGroup& other) const;
		bool operator!=(const TextureGroup& other) const;

	};

}