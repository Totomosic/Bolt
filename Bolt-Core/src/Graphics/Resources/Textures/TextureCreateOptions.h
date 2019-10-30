#pragma once
#include "Texture.h"

namespace Bolt
{

	struct BLT_API TextureCreateOptions
	{
	public:
		WrapMode Wrap = WrapMode::Repeat;
		MagFilter Magnification = MagFilter::Linear;
		MinFilter Minification = MinFilter::Linear;
		Mipmaps MipmapMode = Mipmaps::Enabled;

	};

}