#pragma once
#include "Bolt-Core.h"

namespace Bolt
{

	BLT_API enum class CommandArgType
	{
		Int,
		Float,
		Bool,
		String,
		Ignore
	};

	struct BLT_API CommandArg
	{
	public:
		blt::string Name;
		CommandArgType Type;
		bool IsOptional;
	};

}