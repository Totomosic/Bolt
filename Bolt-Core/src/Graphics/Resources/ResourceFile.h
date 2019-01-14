#pragma once
#include "Bolt-Core.h"

namespace Bolt
{

	BLT_API enum class ResourceType
	{
		Unknown,
		Texture2D,
		Model,
		Animation,
		Data,
		Shader
	};

	struct BLT_API ResourceFile
	{
	public:
		ResourceType Type = ResourceType::Unknown;
		blt::string Name = "";
		id_t Id = (id_t)-1;
		XMLnode Attributes;

	};

}