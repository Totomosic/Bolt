#pragma once
#include "Types.h"

namespace Bolt
{

	struct BLT_API XMLnode
	{
	public:
		blt::string Name;
		std::unordered_map<blt::string, blt::string> Attributes;
		blt::string Data;
		std::vector<XMLnode> Children;
		XMLnode* Parent;

	};

}