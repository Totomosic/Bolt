#include "Texture2D.h"

namespace Bolt::Assets
{

	struct Texture2DHeader
	{
	public:
		int Width;
		int Height;
		PixelFormat Format;

	public:
		void Serialize(void* data)
		{
			int* ptr = (int*)data;
			*ptr = Width;
			++ptr;
			*ptr = Height;
			++ptr;
			memcpy(ptr, &Format, sizeof(Format));
		}

		static size_t GetSize() { return sizeof(int) + sizeof(int) + sizeof(PixelFormat); }
		static Texture2DHeader Deserialize(const void* data)
		{
			Texture2DHeader header;
			const int* ptr = (const int*)data;
			header.Width = *ptr;
			++ptr;
			header.Height = *ptr;
			++ptr;
			memcpy(&header.Format, ptr, sizeof(header.Format));
			return header;
		}
	};

	ByteStream Texture2DEngine::CreateBoltFormat(const std::string& assetName, const Texture2D& texture)
	{
		AssetHeader assetHeader = CreateAssetHeader(assetName);
		Texture2DHeader header;
		header.Width = texture.Width;
		header.Height = texture.Height;
		header.Format = texture.Format;
		int nComponents = GetComponentCount(header.Format);
		size_t assetHeaderLength = AssetHeader::GetSize();
		size_t headerLength = assetHeader.MetadataLength;
		size_t dataLength = ((size_t)header.Width * (size_t)header.Height * (size_t)nComponents);
		size_t length = assetHeaderLength + headerLength + dataLength;
		ByteStream stream(length);

		uint8_t* streamPtr = (uint8_t*)stream.GetData();
		assetHeader.Serialize(streamPtr + 0);
		header.Serialize(streamPtr + assetHeaderLength);
		memcpy(streamPtr + assetHeaderLength + headerLength, texture.Pixels.get(), dataLength);

		return stream;
	}

	Asset<Texture2D> Texture2DEngine::ReadBoltFormat(const void* data, size_t length)
	{
		const uint8_t* ptr = (const uint8_t*)data;
		size_t assetHeaderLength = AssetHeader::GetSize();
		if (length < assetHeaderLength)
		{
			BLT_CORE_ERROR("Length of data is less than size of asset header");
			return {};
		}
		Asset<Texture2D> result;
		AssetHeader header = AssetHeader::Deserialize(data);
		if (length < assetHeaderLength + header.MetadataLength)
		{
			BLT_CORE_ERROR("Invalid Data Format: Length of data is less than size of header");
			return {};
		}
		Texture2DHeader textureHeader = Texture2DHeader::Deserialize(ptr + assetHeaderLength);
		int nComponents = GetComponentCount(textureHeader.Format);
		size_t dataLength = (size_t)textureHeader.Width * (size_t)textureHeader.Height * (size_t)nComponents;
		if (length < assetHeaderLength + header.MetadataLength + dataLength)
		{
			BLT_CORE_ERROR("Invalid Data Format: Length of data cannot contain pixel data");
			return {};
		}

		result.Name = header.Name;
		result.Data.Width = textureHeader.Width;
		result.Data.Height = textureHeader.Height;
		result.Data.Format = textureHeader.Format;
		result.Data.Components = nComponents;
		result.Data.Pixels = std::shared_ptr<uint8_t>(new uint8_t[dataLength / sizeof(uint8_t)]);
		memcpy(result.Data.Pixels.get(), ptr + assetHeaderLength + header.MetadataLength, dataLength);
		return result;
	}

	AssetHeader Texture2DEngine::CreateAssetHeader(const std::string& assetName)
	{
		if (assetName.size() >= MAX_ASSET_NAME_LENGTH)
		{
			BLT_CORE_ERROR("Asset name: {} exceeds max name length {}", assetName, MAX_ASSET_NAME_LENGTH);
		}
		AssetHeader header;
		int nameLength = std::min<int>(MAX_ASSET_NAME_LENGTH - 1, assetName.size());
		memset(header.Name, 0, sizeof(header.Name));
		memcpy(header.Name, assetName.data(), nameLength);
		header.Name[nameLength] = '\0';
		header.Type = AssetType::Texture2D;
		header.MetadataLength = Texture2DHeader::GetSize();
		return header;
	}

}