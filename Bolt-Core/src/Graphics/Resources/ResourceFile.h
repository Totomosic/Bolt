#pragma once
#include "Types.h"

namespace Bolt
{

	enum class ResourceType
	{
		Unknown,
		Image,
		Model,
		Animation,
		Data
	};

	struct BLT_API ResourceFile
	{
	public:
		ResourceType Type;
		blt::string Name;
		mutable id_t Id;
		std::unordered_map<blt::string, blt::string> Attributes;

	};

}