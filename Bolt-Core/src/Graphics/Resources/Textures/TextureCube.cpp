#include "Types.h"
#include "TextureCube.h"

namespace Bolt
{

	TextureCube::TextureCube(int width, int height, TextureFormat format, Mipmaps mipmaps) : Texture(width, height, TextureTarget::TextureCube, format, mipmaps)
	{
		
	}

	TextureCube::TextureCube(int width, int height, Mipmaps mipmaps) : TextureCube(width, height, TextureFormat::RGBA)
	{
	
	}

	TextureCube::TextureCube(const Image& front, const Image& back, const Image& left, const Image& right, const Image& bottom, const Image& top, TextureFormat format, Mipmaps mipmaps) : TextureCube(front.Width, front.Height, format, mipmaps)
	{
		SetImages(front, back, left, right, bottom, top);
		SetMinFilter(MinFilter::Linear);
		SetMagFilter(MagFilter::Linear);
		SetWrapMode(WrapMode::ClampToEdge);
	}

	TextureCube::TextureCube(const Image& front, const Image& back, const Image& left, const Image& right, const Image& bottom, const Image& top, Mipmaps mipmaps) : TextureCube(front, back, left, right, bottom, top, TextureFormat::RGBA, mipmaps)
	{
		
	}

	void TextureCube::SetImages(const Image& front, const Image& back, const Image& left, const Image& right, const Image& bottom, const Image& top) const
	{
		BLT_ASSERT(front.Width == back.Width && back.Width == left.Width && left.Width == right.Width && right.Width == bottom.Width && bottom.Width == top.Width, "All Images must have same Width");
		BLT_ASSERT(front.Height == back.Height && back.Height == left.Height && left.Height == right.Height && right.Height == bottom.Height && bottom.Height == top.Height, "All Images must have same Height");
		Bind();
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA8, front.Width, front.Height, 0, (GLenum)m_Format, GL_UNSIGNED_BYTE, front.Pixels);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA8, back.Width, back.Height, 0, (GLenum)m_Format, GL_UNSIGNED_BYTE, back.Pixels);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA8, right.Width, right.Height, 0, (GLenum)m_Format, GL_UNSIGNED_BYTE, right.Pixels);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA8, left.Width, left.Height, 0, (GLenum)m_Format, GL_UNSIGNED_BYTE, left.Pixels);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA8, top.Width, top.Height, 0, (GLenum)m_Format, GL_UNSIGNED_BYTE, top.Pixels);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA8, bottom.Width, bottom.Height, 0, (GLenum)m_Format, GL_UNSIGNED_BYTE, bottom.Pixels);
		if (HasMipmaps())
		{
			GenerateMipmaps();
		}
	}

}