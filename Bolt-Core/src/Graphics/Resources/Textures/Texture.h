#pragma once
#include "Bolt-Core.h"
#include "..\..\GLshared.h"
#include "..\Resource.h"

namespace Bolt
{

	enum class TextureTarget : GLenum
	{
		Texture1D = GL_TEXTURE_1D,
		Texture2D = GL_TEXTURE_2D,
		Texture3D = GL_TEXTURE_3D,
		TextureCube = GL_TEXTURE_CUBE_MAP
	};

	enum class TextureFormat : GLenum
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

	enum class Mipmaps : GLenum
	{
		Disabled,
		Enabled
	};

	enum class MinFilter : GLenum
	{
		Linear = GL_LINEAR,
		Nearest = GL_NEAREST
	};

	enum class MagFilter : GLenum
	{
		Linear = GL_LINEAR,
		Nearest = GL_NEAREST
	};

	enum class WrapMode : GLenum
	{
		Clamp = GL_CLAMP,
		Repeat = GL_REPEAT,
		ClampToEdge = GL_CLAMP_TO_EDGE
	};

	class BLT_API Texture : public GLshared, public Resource
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
		virtual ~Texture() override;

		int Width() const;
		int Height() const;
		id_t ID() const;
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

	protected:
		void Create();
		void GenerateMipmaps() const;

	protected:
		static TextureFormat GetImageFormat(const Image& image);

	};

}