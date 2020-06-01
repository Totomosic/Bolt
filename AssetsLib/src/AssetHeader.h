#pragma once
#include "Logging.h"

namespace Bolt::Assets
{

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
		std::string Name;
		size_t MetadataLength;
	};

}