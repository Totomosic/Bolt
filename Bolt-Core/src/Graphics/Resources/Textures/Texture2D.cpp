#include "Texture2D.h"

namespace Bolt
{

	Texture2D::Texture2D(int width, int height, TextureFormat format, TextureCreateOptions options) : Texture2D(Image(width, height, 4, nullptr), format, options)
	{
	
	}

	Texture2D::Texture2D(int width, int height, TextureCreateOptions options) : Texture2D(width, height, TextureFormat::RGBA, options)
	{
	
	}

	Texture2D::Texture2D(const Image& image, TextureFormat format, TextureCreateOptions options) : Texture(image.Width, image.Height, TextureTarget::Texture2D, format, options.MipmapMode),
		Pixels(new Color[m_Width * m_Height])
	{
		SetImage(image);
		SetMinFilter(options.Minification);
		SetMagFilter(options.Magnification);
		SetWrapMode(options.Wrap);
	}

	Texture2D::Texture2D(const Image& image, TextureCreateOptions options) : Texture2D(image, GetImageFormat(image), options)
	{
	
	}

	Texture2D::~Texture2D()
	{
		delete[] Pixels;
	}

	void Texture2D::LoadPixels()
	{
		Bind();
		GL_CALL(glGetTexImage((GLenum)m_Target, 0, GL_RGBA, GL_FLOAT, Pixels));
	}

	void Texture2D::UpdatePixels()
	{
		Bind();
		GL_CALL(glTexSubImage2D((GLenum)m_Target, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_FLOAT, Pixels));
		if (HasMipmaps())
		{
			GenerateMipmaps();
		}
	}

	void Texture2D::SetImage(const Image& image) const
	{
		Bind();
		GL_CALL(glTexImage2D((GLenum)m_Target, 0, GL_RGBA8, image.Width, image.Height, 0, (GLenum)m_Format, GL_UNSIGNED_BYTE, image.Pixels));
		if (HasMipmaps())
		{
			GenerateMipmaps();
		}
	}

}