#include "bltpch.h"

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
		Pixels(nullptr)
	{
		SetImage(image);
		SetMinFilter(options.Minification);
		SetMagFilter(options.Magnification);
		SetWrapMode(options.Wrap);
	}

	Texture2D::Texture2D(const Image& image, TextureCreateOptions options) : Texture2D(image, GetImageFormat(image), options)
	{
	
	}

	Texture2D::Texture2D(Texture2D&& other) : Texture(other.Width(), other.Height(), other.m_Target, other.m_Format, other.m_Mipmaps),
		Pixels(other.Pixels)
	{
		other.Pixels = nullptr;
	}

	Texture2D& Texture2D::operator=(Texture2D&& other)
	{
		id_t id = m_Id;
		m_Id = other.m_Id;
		other.m_Id = id;
		m_Width = other.m_Width;
		m_Height = other.m_Height;
		m_Target = other.m_Target;
		m_Format = other.m_Format;
		m_Mipmaps = other.m_Mipmaps;
		Color* pixels = Pixels;
		Pixels = other.Pixels;
		other.Pixels = pixels;
		return *this;
	}

	Texture2D::~Texture2D()
	{
		if (Pixels != nullptr)
		{
			BLT_DELETE_ARR Pixels;
		}
	}

	void Texture2D::LoadPixels() const
	{
		if (Pixels == nullptr)
		{
			Pixels = BLT_NEW Color[m_Width * m_Height];
		}
		Bind();
		GL_CALL(glGetTexImage((GLenum)m_Target, 0, GL_RGBA, GL_FLOAT, Pixels));
	}

	void Texture2D::UpdatePixels(bool deletePixels) const
	{
		BLT_ASSERT(Pixels != nullptr, "Cnanot update pixels as they have not been loaded, Use LoadPixels()");
		Bind();
		GL_CALL(glTexSubImage2D((GLenum)m_Target, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_FLOAT, Pixels));
		if (HasMipmaps())
		{
			GenerateMipmaps();
		}
		if (deletePixels)
		{
			BLT_DELETE_ARR Pixels;
			Pixels = nullptr;
		}
	}

	Image Texture2D::GetImage() const
	{
		Image image;
		image.Width = Width();
		image.Height = Height();
		image.Components = 4;
		image.Pixels = BLT_NEW byte[image.Width * image.Height * image.Components];
		Bind();
		GL_CALL(glGetTexImage((GLenum)m_Target, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.Pixels));
		return image;
	}

	Image Texture2D::GetImage(int x, int y, int w, int h) const
	{
		Image image = GetImage();
		Image result;
		result.Width = w;
		result.Height = h;
		result.Components = 4;
		result.Pixels = BLT_NEW byte[result.Width * result.Height * result.Components];
		for (int i = y; i < y + h; i++)
		{
			memcpy(result.Pixels + (i - y) * result.Width * 4, image.Pixels + x * 4 + i * image.Width * 4, w * 4);
		}
		return result;
	}

	void Texture2D::SetPixel(int x, int y, const Color& color)
	{
		BLT_ASSERT(IsValidXY(x, y), "Pixel does not exists, texture dimensions:(", Width(), ", ", Height(), ")");
		Bind();
		GL_CALL(glTexSubImage2D((GLenum)m_Target, 0, x, y, 1, 1, GL_RGBA, GL_FLOAT, &color));
		if (HasMipmaps())
		{
			GenerateMipmaps();
		}
	}

	void Texture2D::SetRegion(int x, int y, int w, int h, const Color& color)
	{
		Image image;
		image.Width = w;
		image.Height = h;
		image.Components = 4;
		image.Pixels = BLT_NEW byte[w * h * 4];
		Vector4<byte> bytes = color.ToBytes();
		for (int i = 0; i < w * h; i++)
		{
			memcpy(image.Pixels + i * 4, &bytes, 4);
		}
		SetRegion(x, y, w, h, image);
		BLT_DELETE_ARR image.ReleasePixels();
	}

	void Texture2D::SetRegion(int x, int y, int w, int h, const Image& image)
	{
		BLT_ASSERT(IsValidXY(x, y), "Pixels do not exist, texture dimensions:(", Width(), ", ", Height(), ")");
		BLT_ASSERT(IsValidXY(x + w - 1, y + h - 1), "Pixels do not exist, texture dimensions:(", Width(), ", ", Height(), ")");
		BLT_ASSERT(w == image.Width && h == image.Height, "Invalid image dimensions");
		Bind();
		GL_CALL(glTexSubImage2D((GLenum)m_Target, 0, x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, image.Pixels));
		if (HasMipmaps())
		{
			GenerateMipmaps();
		}
	}

	void Texture2D::SetRegion(int x, int y, int w, int h, const Image& image, ResizeFilter filter)
	{
		if (image.Width == w && image.Height == h)
		{
			SetRegion(x, y, w, h, image);
		}
		else
		{
			Image i = image.Resize(w, h, filter);
			SetRegion(x, y, w, h, i);
		}
	}

	void Texture2D::SetRegion(int x, int y, int w, int h, const Image& image, int ix, int iy, int iw, int ih, ResizeFilter filter)
	{
		BLT_ASSERT(ix >= 0 && iy >= 0 && ix < image.Width && iy < image.Height, "Pixels do not exist, image dimensions: (", image.Width, ", ", image.Height, ")");
		BLT_ASSERT(iw > 0 && ih > 0 && ix + iw <= image.Width && iy + ih <= image.Height, "Pixels do not exist, image dimensions: (", image.Width, ", ", image.Height, ")");
		Image im;
		im.Width = iw;
		im.Height = ih;
		im.Components = 4;
		im.Pixels = BLT_NEW byte[im.Width * im.Height * im.Components];
		for (int i = iy; i < iy + ih; i++)
		{
			memcpy(im.Pixels + iw * (i - iy) * 4, image.Pixels + ix * 4 + image.Width * i * 4, iw * 4);
		}		
		if (!(iw == w && ih == h))
		{
			im = im.Resize(w, h, filter);
		}
		SetRegion(x, y, w, h, im);
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

	bool Texture2D::IsValidXY(int x, int y) const
	{
		return (x >= 0 && y >= 0) && (x < Width() && y < Height());
	}

}