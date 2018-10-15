#pragma once
#include "Types.h"
#include "ResourceFile.h"

namespace Bolt
{

	struct BLT_API ResourcePack
	{
	public:
		std::unordered_map<blt::string, ResourceFile> Resources;

	};

}