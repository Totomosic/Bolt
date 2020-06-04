#pragma once
#include "AssetHeader.h"
#include "ByteStream.h"

namespace Bolt::Assets
{

	enum class PixelFormat : uint8_t
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

	struct BLT_API Texture2D
	{
	public:
		int Width;
		int Height;
		int Components;
		PixelFormat Format;
		std::shared_ptr<uint8_t> Pixels;
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

	class BLT_API Texture2DEngine
	{
	public:
		static ByteStream CreateBoltFormat(const std::string& assetName, const Texture2D& texture);
		static Asset<Texture2D> ReadBoltFormat(const void* data, size_t length);

	private:
		static AssetHeader CreateAssetHeader(const std::string& assetName);
	};

}