#pragma once
#include "../Resource.h"
#include "AssetsLib/Image2D.h"

namespace Bolt
{

	class BLT_API Texture : public Resource
	{
	public:
		BLT_API enum class StorageType : GLenum
		{
			Float = GL_FLOAT,
			UnsignedByte = GL_UNSIGNED_BYTE
		};

	protected:
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
			RGB = GL_RGB,
			RGBA = GL_RGBA,
			BGR = GL_BGR,
			BGRA = GL_BGRA
		};

		BLT_API enum class Filter : GLenum
		{
			Linear = GL_LINEAR,
			Nearest = GL_NEAREST
		};

		BLT_API enum class WrapMode : GLenum
		{
			Repeat = GL_REPEAT,
			ClampToEdge = GL_CLAMP_TO_EDGE
		};

	protected:
		id_t m_Id;
		int m_Width;
		int m_Height;
		TextureTarget m_Target;
		TextureFormat m_Format;
		mutable bool m_HasMipmaps;

	public:
		Texture(int width, int height, TextureTarget target, TextureFormat format, bool generateMipmaps);
		Texture(const Texture& other) = delete;
		Texture& operator=(const Texture& other) = delete;
		Texture(Texture&& other);
		Texture& operator=(Texture&& other);
		virtual ~Texture() override;

		int Width() const;
		int Height() const;
		id_t Id() const;
		TextureTarget Target() const;
		PixelFormat Format() const;
		int MipmapCount() const;
		bool HasMipmaps() const;

		void SetMinFilter(PixelFilter filter) const;
		void SetMagFilter(PixelFilter fitler) const;
		void SetWrapMode(PixelWrap mode) const;

		virtual void Bind(int textureBank = 0) const;
		virtual void Unbind(int textureBank = 0) const;

		virtual void Download(void* buffer, StorageType type, int level = 0) const;
		virtual void Upload(const void* data, int x, int y, int width, int height, StorageType type, int level = 0) const;

		std::unique_ptr<Resource> Clone() const override;

	protected:
		void Create();
		void GenerateMipmaps() const;

		inline static TextureFormat PixelFormatToTextureFormat(PixelFormat format)
		{
			switch (format)
			{
			case PixelFormat::R:
				return TextureFormat::R;
			case PixelFormat::G:
				return TextureFormat::G;
			case PixelFormat::B:
				return TextureFormat::B;
			case PixelFormat::A:
				return TextureFormat::A;
			case PixelFormat::RGB:
				return TextureFormat::RGB;
			case PixelFormat::RGBA:
				return TextureFormat::RGBA;
			case PixelFormat::BGR:
				return TextureFormat::BGR;
			case PixelFormat::BGRA:
				return TextureFormat::BGRA;
			default:
				break;
			}
			BLT_ASSERT(false, "Invalid pixel format");
			return TextureFormat::RGBA;
		}

		inline static PixelFormat TextureFormatToPixelFormat(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::R:
				return PixelFormat::R;
			case TextureFormat::G:
				return PixelFormat::G;
			case TextureFormat::B:
				return PixelFormat::B;
			case TextureFormat::A:
				return PixelFormat::A;
			case TextureFormat::RGB:
				return PixelFormat::RGB;
			case TextureFormat::RGBA:
				return PixelFormat::RGBA;
			case TextureFormat::BGR:
				return PixelFormat::BGR;
			case TextureFormat::BGRA:
				return PixelFormat::BGRA;
			default:
				break;
			}
			BLT_ASSERT(false, "Invalid pixel format");
			return PixelFormat::RGBA;
		}

		inline static Filter PixelFilterToFilter(PixelFilter filter)
		{
			switch (filter)
			{
			case PixelFilter::Nearest:
				return Filter::Nearest;
			case PixelFilter::Linear:
				return Filter::Linear;
			}
			return Filter::Linear;
		}

		inline static WrapMode PixelWrapToWrapMode(PixelWrap wrap)
		{
			switch (wrap)
			{
			case PixelWrap::Clamp:
				return WrapMode::ClampToEdge;
			case PixelWrap::Repeat:
				return WrapMode::Repeat;
			}
			return WrapMode::Repeat;
		}

	};

	

}