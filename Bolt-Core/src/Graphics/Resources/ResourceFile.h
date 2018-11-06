#pragma once
#include "Types.h"

namespace Bolt
{

	BLT_API enum class ResourceType
	{
		Unknown,
		Texture2D,
		Model,
		Animation,
		Data
	};

	struct BLT_API ResourceFile
	{
	public:
		ResourceType Type;
		blt::string Name;
		id_t Id;
		XMLnode Attributes;

	};

}