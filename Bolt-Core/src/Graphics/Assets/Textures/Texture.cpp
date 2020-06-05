#include "bltpch.h"

#include "Texture.h"

namespace Bolt
{

	Texture::Texture(int width, int height, TextureTarget target, TextureFormat format, bool generateMipmaps) : Resource(),
		m_Id(0), m_Width(width), m_Height(height), m_Target(target), m_Format(format), m_HasMipmaps(generateMipmaps)
	{
		Create();
	}

	Texture::Texture(Texture&& other)
		: m_Id(other.m_Id), m_Width(other.m_Width), m_Height(other.m_Height), m_Target(other.m_Target), m_Format(other.m_Format), m_HasMipmaps(other.m_HasMipmaps)
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
		m_HasMipmaps = other.m_HasMipmaps;
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

	id_t Texture::Id() const
	{
		return m_Id;
	}

	Texture::TextureTarget Texture::Target() const
	{
		return m_Target;
	}
	
	PixelFormat Texture::Format() const
	{
		return TextureFormatToPixelFormat(m_Format);
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
		return m_HasMipmaps;
	}

	void Texture::SetMinFilter(PixelFilter filter) const
	{
		Bind();
		GLenum value = (HasMipmaps()) ? ((filter == PixelFilter::Linear) ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR) : (GLenum)PixelFilterToFilter(filter);
		GL_CALL(glTexParameteri((GLenum)m_Target, GL_TEXTURE_MIN_FILTER, value));
	}

	void Texture::SetMagFilter(PixelFilter filter) const
	{
		Bind();
		GL_CALL(glTexParameteri((GLenum)m_Target, GL_TEXTURE_MAG_FILTER, (GLenum)PixelFilterToFilter(filter)));
	}

	void Texture::SetWrapMode(PixelWrap mode) const
	{
		Bind();
		WrapMode oglMode = PixelWrapToWrapMode(mode);
		GL_CALL(glTexParameteri((GLenum)m_Target, GL_TEXTURE_WRAP_R, (GLenum)oglMode));
		GL_CALL(glTexParameteri((GLenum)m_Target, GL_TEXTURE_WRAP_S, (GLenum)oglMode));
		GL_CALL(glTexParameteri((GLenum)m_Target, GL_TEXTURE_WRAP_T, (GLenum)oglMode));
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
		std::unique_ptr<Texture> texture = std::make_unique<Texture>(m_Width, m_Height, m_Target, m_Format, m_HasMipmaps);
		float* imageData = BLT_NEW float[m_Width * m_Height * 4];
		Bind();
		Download(imageData, StorageType::Float);
		texture->Bind();
		texture->Upload(imageData, 0, 0, m_Width, m_Height, StorageType::Float);
		if (m_HasMipmaps)
		{
			texture->GenerateMipmaps();
		}
		BLT_DELETE_ARR imageData;
		return texture;
	}
	
	void Texture::Create()
	{
		GL_CALL(glGenTextures(1, &m_Id));
		Bind();
		GL_CALL(glTexImage2D((GLenum)Target(), 0, GL_RGBA8, m_Width, m_Height, 0, (GLenum)m_Format, GL_UNSIGNED_BYTE, nullptr));
	}

	void Texture::GenerateMipmaps() const
	{
		m_HasMipmaps = true;
		Bind();
		GL_CALL(glGenerateMipmap((GLenum)m_Target));
	}

}