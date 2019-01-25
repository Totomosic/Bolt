#include "Types.h"

#include "Texture.h"

namespace Bolt
{

	Texture::Texture(int width, int height, TextureTarget target, TextureFormat format, Mipmaps mipmap) : GLshared(), Resource(),
		m_Id(0), m_Width(width), m_Height(height), m_Target(target), m_Format(format), m_Mipmaps(mipmap)
	{
		Create();
	}

	Texture::Texture(Texture&& other)
		: m_Id(other.m_Id), m_Width(other.m_Width), m_Height(other.m_Height), m_Target(other.m_Target), m_Format(other.m_Format), m_Mipmaps(other.m_Mipmaps)
	{
		other.m_Id = 0;
	}

	Texture& Texture::operator=(Texture&& other)
	{
		id_t id = m_Id;
		m_Id = other.m_Id;
		other.m_Id = id;
		m_Width = other.m_Width;
		m_Height = other.m_Height;
		m_Target = other.m_Target;
		m_Format = other.m_Format;
		m_Mipmaps = other.m_Mipmaps;
		return *this;
	}

	Texture::~Texture()
	{
		if (m_Id != 0)
		{
			GL_CALL(glDeleteTextures(1, &m_Id));
		}
	}

	int Texture::Width() const
	{
		return m_Width;
	}

	int Texture::Height() const
	{
		return m_Height;
	}

	id_t Texture::ID() const
	{
		return m_Id;
	}

	TextureTarget Texture::Target() const
	{
		return m_Target;
	}
	
	TextureFormat Texture::Format() const
	{
		return m_Format;
	}

	Mipmaps Texture::MipmapMode() const
	{
		return m_Mipmaps;
	}

	int Texture::MipmapCount() const
	{
		if (!HasMipmaps())
		{
			return 1;
		}
		return (int)log2(std::min(m_Width, m_Height));
	}

	bool Texture::HasMipmaps() const
	{
		return m_Mipmaps == Mipmaps::Enabled;
	}

	void Texture::SetMinFilter(MinFilter filter) const
	{
		Bind();
		GLenum value = (HasMipmaps()) ? (filter == MinFilter::Linear) ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR : (GLenum)filter;
		GL_CALL(glTexParameteri((GLenum)m_Target, GL_TEXTURE_MIN_FILTER, value));
	}

	void Texture::SetMagFilter(MagFilter filter) const
	{
		Bind();
		GL_CALL(glTexParameteri((GLenum)m_Target, GL_TEXTURE_MAG_FILTER, (GLenum)filter));
	}

	void Texture::SetWrapMode(WrapMode mode) const
	{
		Bind();
		GL_CALL(glTexParameteri((GLenum)m_Target, GL_TEXTURE_WRAP_R, (GLenum)mode));
		GL_CALL(glTexParameteri((GLenum)m_Target, GL_TEXTURE_WRAP_S, (GLenum)mode));
		GL_CALL(glTexParameteri((GLenum)m_Target, GL_TEXTURE_WRAP_T, (GLenum)mode));
	}

	void Texture::Bind(int textureBank) const
	{
		GL_CALL(glActiveTexture(GL_TEXTURE0 + textureBank));
		GL_CALL(glBindTexture((GLenum)Target(), m_Id));
	}

	void Texture::Unbind(int textureBank) const
	{
		GL_CALL(glActiveTexture(GL_TEXTURE0 + textureBank));
		GL_CALL(glBindTexture((GLenum)Target(), 0));
	}

	void Texture::Download(void* buffer, StorageType type, int level) const
	{
		Bind();
		GL_CALL(glGetTexImage((GLenum)Target(), level, GL_RGBA, (GLenum)type, buffer));
	}

	void Texture::Upload(const void* data, int x, int y, int width, int height, StorageType type, int level) const
	{
		Bind();
		GL_CALL(glTexSubImage2D((GLenum)Target(), level, x, y, width, height, GL_RGBA, (GLenum)type, data));
	}

	std::unique_ptr<Resource> Texture::Clone() const
	{
		std::unique_ptr<Texture> texture = std::make_unique<Texture>(m_Width, m_Height, m_Target, m_Format, m_Mipmaps);
		float* imageData = new float[m_Width * m_Height * 4];
		Bind();
		Download(imageData, StorageType::Float);
		texture->Bind();
		texture->Upload(imageData, 0, 0, m_Width, m_Height, StorageType::Float);
		if (m_Mipmaps == Mipmaps::Enabled)
		{
			texture->GenerateMipmaps();
		}
		delete[] imageData;
		return std::move(texture);
	}
	
	void Texture::Create()
	{
		GL_CALL(glGenTextures(1, &m_Id));
		Bind();
		GL_CALL(glTexImage2D((GLenum)Target(), 0, GL_RGBA8, m_Width, m_Height, 0, (GLenum)m_Format, GL_UNSIGNED_BYTE, nullptr));
	}

	void Texture::GenerateMipmaps() const
	{
		m_Mipmaps = Mipmaps::Enabled;
		Bind();
		GL_CALL(glGenerateMipmap((GLenum)m_Target));
	}

	TextureFormat Texture::GetImageFormat(const Image& image)
	{
		switch (image.Components)
		{
		case 4:
			return TextureFormat::RGBA;
		case 3:
			return TextureFormat::RGB;
		case 2:
			return TextureFormat::RG;
		case 1:
			return TextureFormat::R;
		default:
			return TextureFormat::RGBA;
		}
	}

}