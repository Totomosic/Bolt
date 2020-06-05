#pragma once
#include "Texture.h"
#include "AssetsLib/Image2D.h"

namespace Bolt
{

	class TextureCube : public Texture
	{
	private:

	public:
		TextureCube(int width, int height, TextureFormat format, bool generateMipmaps = true);
		TextureCube(int width, int height, bool generateMipmaps = true);
		TextureCube(const Image2D& front, const Image2D& back, const Image2D& left, const Image2D& right, const Image2D& bottom, const Image2D& top,
			TextureFormat format, bool generateMipmaps = true);
		TextureCube(const Image2D& front, const Image2D& back, const Image2D& left, const Image2D& right, const Image2D& bottom, const Image2D& top,
			bool generateMipmaps = true);

	private:
		void SetImages(const Image2D& front, const Image2D& back, const Image2D& left, const Image2D& right, const Image2D& bottom, const Image2D& top) const;

	};

}