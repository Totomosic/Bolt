#include "bltpch.h"
#include "Texture2D.h"

namespace Bolt
{

	Texture2D::Texture2D(int width, int height, PixelFormat format, Image2D::Options options) : Texture2D(Image2D{ width, height, format, nullptr, options })
	{
	
	}

	Texture2D::Texture2D(int width, int height, Image2D::Options options) : Texture2D(width, height, PixelFormat::RGBA, options)
	{
	
	}

	Texture2D::Texture2D(int width, int height, const void* pixels, PixelFormat format, Image2D::Options options) : Texture(width, height, TextureTarget::Texture2D, PixelFormatToTextureFormat(format), options.GenerateMipmaps),
		Pixels(nullptr)
	{
		SetImage(pixels);
	}

	Texture2D::Texture2D(const Image2D& image) : Texture(image.Width, image.Height, TextureTarget::Texture2D, PixelFormatToTextureFormat(image.Format), image.Parameters.GenerateMipmaps),
		Pixels(nullptr)
	{
		SetImage((const void*)image.Pixels.get());
		SetMinFilter(image.Parameters.Minification);
		SetMagFilter(image.Parameters.Magnification);
		SetWrapMode(image.Parameters.Wrap);
	}

	Texture2D::Texture2D(Texture2D&& other) : Texture(other.Width(), other.Height(), other.m_Target, other.m_Format, other.m_HasMipmaps),
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
		m_HasMipmaps = other.m_HasMipmaps;
		Color* pixels = Pixels;
		Pixels = other.Pixels;
		other.Pixels = pixels;
		return *this;
	}

	Texture2D::~Texture2D()
	{
		BLT_DELETE_ARR Pixels;
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

	Image2D Texture2D::GetImage() const
	{
		Image2D image;
		image.Width = Width();
		image.Height = Height();
		image.Format = PixelFormat::RGBA;
		int components = GetComponentCount(image.Format);
		image.Pixels = std::shared_ptr<uint8_t>(BLT_NEW byte[(size_t)image.Width * (size_t)image.Height * (size_t)components]);
		Bind();
		GL_CALL(glGetTexImage((GLenum)m_Target, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.Pixels.get()));
		return image;
	}

	Image2D Texture2D::GetImage(int x, int y, int w, int h) const
	{
		Image2D image = GetImage();
		Image2D result;
		result.Width = w;
		result.Height = h;
		result.Format = PixelFormat::RGBA;
		int components = GetComponentCount(result.Format);
		result.Pixels = std::shared_ptr<uint8_t>(BLT_NEW byte[(size_t)result.Width * (size_t)result.Height * (size_t)components]);
		for (int i = y; i < y + h; i++)
		{
			memcpy(result.Pixels.get() + (size_t)(i - y) * (size_t)result.Width * 4, image.Pixels.get() + (size_t)x * 4 + (size_t)i * (size_t)image.Width * 4, (size_t)w * 4);
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
		Image2D image;
		image.Width = w;
		image.Height = h;
		image.Format = PixelFormat::RGBA;
		image.Pixels = std::shared_ptr<uint8_t>(BLT_NEW byte[(size_t)w * (size_t)h * 4]);
		Vector4<byte> bytes = color.ToBytes();
		for (int i = 0; i < w * h; i++)
		{
			memcpy(image.Pixels.get() + (size_t)i * 4, &bytes, 4);
		}
		SetRegion(x, y, w, h, image);
	}

	void Texture2D::SetRegion(int x, int y, int w, int h, const Image2D& image)
	{
		BLT_ASSERT(IsValidXY(x, y), "Pixels do not exist, texture dimensions:(", Width(), ", ", Height(), ")");
		BLT_ASSERT(IsValidXY(x + w - 1, y + h - 1), "Pixels do not exist, texture dimensions:(", Width(), ", ", Height(), ")");
		BLT_ASSERT(w == image.Width && h == image.Height, "Invalid image dimensions");
		Bind();
		GL_CALL(glTexSubImage2D((GLenum)m_Target, 0, x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, image.Pixels.get()));
		if (HasMipmaps())
		{
			GenerateMipmaps();
		}
	}

	void Texture2D::SetImage(const void* pixels) const
	{
		Bind();
		GL_CALL(glTexImage2D((GLenum)m_Target, 0, GL_RGBA8, m_Width, m_Height, 0, (GLenum)m_Format, GL_UNSIGNED_BYTE, pixels));
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