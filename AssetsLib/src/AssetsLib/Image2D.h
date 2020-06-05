#pragma once
#include "AssetHeader.h"
#include "ByteStream.h"

namespace Bolt
{

	BLT_API enum class PixelFormat : uint8_t
	{
		R,
		G,
		B,
		A,
		RGB,
		BGR,
		RGBA,
		BGRA
	};

	BLT_API enum class PixelFilter : uint8_t
	{
		Nearest,
		Linear
	};

	BLT_API enum class PixelWrap : uint8_t
	{
		Repeat,
		Clamp
	};

	inline int GetComponentCount(PixelFormat format)
	{
		switch (format)
		{
		case PixelFormat::R:
			return 1;
		case PixelFormat::G:
			return 1;
		case PixelFormat::B:
			return 1;
		case PixelFormat::A:
			return 1;
		case PixelFormat::RGB:
			return 3;
		case PixelFormat::BGR:
			return 3;
		case PixelFormat::RGBA:
			return 4;
		case PixelFormat::BGRA:
			return 4;
		}
		return 1;
	}

	struct BLT_API Image2D
	{
	public:
		struct BLT_API Options
		{
		public:
			PixelWrap Wrap = PixelWrap::Repeat;
			PixelFilter Magnification = PixelFilter::Linear;
			PixelFilter Minification = PixelFilter::Linear;
			bool GenerateMipmaps = true;
		};

	public:
		int Width;
		int Height;
		PixelFormat Format;
		std::shared_ptr<uint8_t> Pixels;
		Image2D::Options Parameters;
	};

	inline constexpr size_t GetOptionsSize()
	{
		return sizeof(Image2D::Options::Wrap) + sizeof(Image2D::Options::Magnification) + sizeof(Image2D::Options::Minification) + sizeof(Image2D::Options::GenerateMipmaps);
	}

	inline void SerializeOptions(const Image2D::Options& options, void* data)
	{
		uint8_t* ptr = (uint8_t*)data;
		memcpy(ptr + 0, &options.Wrap, sizeof(options.Wrap));
		memcpy(ptr + sizeof(options.Wrap), &options.Magnification, sizeof(options.Magnification));
		memcpy(ptr + sizeof(options.Wrap) + sizeof(options.Magnification), &options.Minification, sizeof(options.Minification));
		memcpy(ptr + sizeof(options.Wrap) + sizeof(options.Magnification) + sizeof(options.Minification), &options.GenerateMipmaps, sizeof(options.GenerateMipmaps));
	}

	inline Image2D::Options DeserializeOptions(const void* data)
	{
		const uint8_t* ptr = (const uint8_t*)data;
		Image2D::Options result;
		memcpy(&result.Wrap, ptr + 0, sizeof(result.Wrap));
		memcpy(&result.Magnification, ptr + sizeof(result.Wrap), sizeof(result.Magnification));
		memcpy(&result.Minification, ptr + sizeof(result.Wrap) + sizeof(result.Magnification), sizeof(result.Minification));
		memcpy(&result.GenerateMipmaps, ptr + sizeof(result.Wrap) + sizeof(result.Magnification) + sizeof(result.Minification), sizeof(result.GenerateMipmaps));
		return result;
	}

}

namespace Bolt::Assets
{

	class BLT_API Texture2DEngine
	{
	public:
		static ByteStream CreateBoltFormat(const std::string& assetName, const Image2D& texture);
		static Asset<Image2D> ReadBoltFormat(const void* data, size_t length);

	private:
		static AssetHeader CreateAssetHeader(const std::string& assetName);
	};

}