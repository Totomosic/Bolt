#pragma once
#include "Core/Utils/Filesystem/XML/XMLnode.h"

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
		std::string Name = "";
		id_t Id = (id_t)-1;
		XMLnode Attributes;

	};

}