#pragma once
#include "Bolt-Core.h"
#include "..\Resource.h"

namespace Bolt
{

	BLT_API enum class StorageType : GLenum
	{
		Float = GL_FLOAT,
		UnsignedByte = GL_UNSIGNED_BYTE
	};

	BLT_API enum class TextureTarget : GLenum
	{
		Texture1D = GL_TEXTURE_1D,
		Texture2D = GL_TEXTURE_2D,
		Texture3D = GL_TEXTURE_3D,
		TextureCube = GL_TEXTURE_CUBE_MAP
	};

	BLT_API enum class TextureFormat : GLenum
	{
		R = GL_RED,
		G = GL_GREEN,
		B = GL_BLUE,
		A = GL_ALPHA,
		RG = GL_RG,
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		BGR = GL_BGR,
		BGRA = GL_BGRA
	};

	BLT_API enum class Mipmaps : GLenum
	{
		Disabled,
		Enabled
	};

	BLT_API enum class MinFilter : GLenum
	{
		Linear = GL_LINEAR,
		Nearest = GL_NEAREST
	};

	BLT_API enum class MagFilter : GLenum
	{
		Linear = GL_LINEAR,
		Nearest = GL_NEAREST
	};

	BLT_API enum class WrapMode : GLenum
	{
		Clamp = GL_CLAMP,
		Repeat = GL_REPEAT,
		ClampToEdge = GL_CLAMP_TO_EDGE
	};

	class BLT_API Texture : public Resource
	{
	protected:
		id_t m_Id;
		int m_Width;
		int m_Height;
		TextureTarget m_Target;
		TextureFormat m_Format;
		mutable Mipmaps m_Mipmaps;

	public:
		Texture(int width, int height, TextureTarget target, TextureFormat format, Mipmaps mipmap);
		Texture(const Texture& other) = delete;
		Texture& operator=(const Texture& other) = delete;
		Texture(Texture&& other);
		Texture& operator=(Texture&& other);
		virtual ~Texture() override;

		int Width() const;
		int Height() const;
		id_t Id() const;
		TextureTarget Target() const;
		TextureFormat Format() const;
		Mipmaps MipmapMode() const;
		int MipmapCount() const;
		bool HasMipmaps() const;

		void SetMinFilter(MinFilter filter) const;
		void SetMagFilter(MagFilter fitler) const;
		void SetWrapMode(WrapMode mode) const;

		virtual void Bind(int textureBank = 0) const;
		virtual void Unbind(int textureBank = 0) const;

		virtual void Download(void* buffer, StorageType type, int level = 0) const;
		virtual void Upload(const void* data, int x, int y, int width, int height, StorageType type, int level = 0) const;

		std::unique_ptr<Resource> Clone() const override;

	protected:
		void Create();
		void GenerateMipmaps() const;

	protected:
		static TextureFormat GetImageFormat(const Image& image);

	};

}