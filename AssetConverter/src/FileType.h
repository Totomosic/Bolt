#pragma once
#include "AssetHeader.h"

namespace Bolt::Assets
{

	enum class FileType : uint8_t
	{
		Unknown,

		PNG,
		JPEG,
		JPG,

		OBJ,

		SHADER,
		GLSL,
	};

	extern std::unordered_map<std::string, FileType> s_ExtensionFileTypeMap;

	inline AssetType FileTypeToAssetType(FileType type)
	{
		switch (type)
		{
		case FileType::PNG:
			return AssetType::Texture2D;
		case FileType::JPEG:
			return AssetType::Texture2D;
		case FileType::JPG:
			return AssetType::Texture2D;
		case FileType::OBJ:
			return AssetType::Model;
		case FileType::SHADER:
			return AssetType::Shader;
		case FileType::GLSL:
			return AssetType::Shader;
		default:
			break;
		}
		return AssetType::Unknown;
	}

	inline FileType GetFileTypeFromExtension(const std::string& extension)
	{
		auto it = s_ExtensionFileTypeMap.find(extension);
		if (it != s_ExtensionFileTypeMap.end())
		{
			return it->second;
		}
		return FileType::Unknown;
	}

}