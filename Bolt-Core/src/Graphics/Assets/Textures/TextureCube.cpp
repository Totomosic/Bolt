#include "bltpch.h"

#include "TextureCube.h"

namespace Bolt
{

	TextureCube::TextureCube(int width, int height, TextureFormat format, bool generateMipmaps) : Texture(width, height, TextureTarget::TextureCube, format, generateMipmaps)
	{
		
	}

	TextureCube::TextureCube(int width, int height, bool generateMipmaps) : TextureCube(width, height, TextureFormat::RGBA, generateMipmaps)
	{
	
	}

	TextureCube::TextureCube(const Image2D& front, const Image2D& back, const Image2D& left, const Image2D& right, const Image2D& bottom, const Image2D& top,
		TextureFormat format, bool generateMipmaps) : TextureCube(front.Width, front.Height, format, generateMipmaps)
	{
		SetImages(front, back, left, right, bottom, top);
		SetMinFilter(PixelFilter::Linear);
		SetMagFilter(PixelFilter::Linear);
		SetWrapMode(PixelWrap::Clamp);
	}

	TextureCube::TextureCube(const Image2D& front, const Image2D& back, const Image2D& left, const Image2D& right, const Image2D& bottom, const Image2D& top,
		bool generateMipmaps) : TextureCube(front, back, left, right, bottom, top, TextureFormat::RGBA, generateMipmaps)
	{
		
	}

	void TextureCube::SetImages(const Image2D& front, const Image2D& back, const Image2D& left, const Image2D& right, const Image2D& bottom, const Image2D& top) const
	{
		BLT_ASSERT(front.Width == back.Width && back.Width == left.Width && left.Width == right.Width && right.Width == bottom.Width && bottom.Width == top.Width, "All Images must have same Width");
		BLT_ASSERT(front.Height == back.Height && back.Height == left.Height && left.Height == right.Height && right.Height == bottom.Height && bottom.Height == top.Height, "All Images must have same Height");
		Bind();
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA8, front.Width, front.Height, 0, (GLenum)m_Format, GL_UNSIGNED_BYTE, front.Pixels.get());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA8, back.Width, back.Height, 0, (GLenum)m_Format, GL_UNSIGNED_BYTE, back.Pixels.get());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA8, right.Width, right.Height, 0, (GLenum)m_Format, GL_UNSIGNED_BYTE, right.Pixels.get());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA8, left.Width, left.Height, 0, (GLenum)m_Format, GL_UNSIGNED_BYTE, left.Pixels.get());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA8, top.Width, top.Height, 0, (GLenum)m_Format, GL_UNSIGNED_BYTE, top.Pixels.get());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA8, bottom.Width, bottom.Height, 0, (GLenum)m_Format, GL_UNSIGNED_BYTE, bottom.Pixels.get());
		if (HasMipmaps())
		{
			GenerateMipmaps();
		}
	}

}