#include "TextureGroup.h"

namespace Bolt
{

	TextureGroup::TextureGroup(const TextureGroup& other)
		: Textures(other.Textures)
	{
		for (const auto& pair : other.Animators)
		{
			Animators[pair.first] = pair.second->Clone();
		}
	}

	TextureGroup& TextureGroup::operator=(const TextureGroup& other)
	{
		Textures = other.Textures;
		for (const auto& pair : other.Animators)
		{
			Animators[pair.first] = pair.second->Clone();
		}
		return *this;
	}

	bool TextureGroup::operator==(const TextureGroup& other) const
	{
		return (Textures == other.Textures && Animators == other.Animators);
	}

	bool TextureGroup::operator!=(const TextureGroup& other) const
	{
		return !(*this == other);
	}

}