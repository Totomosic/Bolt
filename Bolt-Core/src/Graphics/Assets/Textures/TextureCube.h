#pragma once
#include "Texture.h"

namespace Bolt
{

	class TextureCube : public Texture
	{
	private:

	public:
		TextureCube(int width, int height, TextureFormat format, Mipmaps mipmaps = Mipmaps::Enabled);
		TextureCube(int width, int height, Mipmaps mipmaps = Mipmaps::Enabled);
		TextureCube(const Image& front, const Image& back, const Image& left, const Image& right, const Image& bottom, const Image& top, TextureFormat format, Mipmaps mipmaps = Mipmaps::Enabled);
		TextureCube(const Image& front, const Image& back, const Image& left, const Image& right, const Image& bottom, const Image& top, Mipmaps mipmaps = Mipmaps::Enabled);

	private:
		void SetImages(const Image& front, const Image& back, const Image& left, const Image& right, const Image& bottom, const Image& top) const;

	};

}