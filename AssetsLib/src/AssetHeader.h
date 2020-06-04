#pragma once
#include "Logging.h"

namespace Bolt::Assets
{

	constexpr int MAX_ASSET_NAME_LENGTH = 64;

	BLT_API enum class AssetType : uint8_t
	{
		Unknown,
		Texture2D,
		Model,
		AnimatedModel,
		Shader
	};

	struct BLT_API AssetHeader
	{
	public:
		AssetType Type;
		char Name[MAX_ASSET_NAME_LENGTH];
		size_t MetadataLength;

	public:
		inline void Serialize(void* data)
		{
			uint8_t* bytePtr = (uint8_t*)data;
			memcpy(bytePtr + 0, &Type, sizeof(Type));
			memcpy(bytePtr + sizeof(Type), Name, sizeof(Name));
			memcpy(bytePtr + sizeof(Type) + sizeof(Name), &MetadataLength, sizeof(MetadataLength));
		}

		inline static size_t GetSize() { return sizeof(Type) + sizeof(Name) + sizeof(MetadataLength); }
		inline static AssetHeader Deserialize(const void* data)
		{
			AssetHeader header;
			const uint8_t* bytePtr = (const uint8_t*)data;
			memcpy(&header.Type, bytePtr + 0, sizeof(header.Type));
			memcpy(header.Name, bytePtr + sizeof(header.Type), sizeof(header.Name));
			memcpy(&header.MetadataLength, bytePtr + sizeof(header.Type) + sizeof(header.Name), sizeof(header.MetadataLength));
			return header;
		}
	};

	template<typename T>
	struct BLT_API Asset
	{
	public:
		std::string Name;
		T Data;
	};

}