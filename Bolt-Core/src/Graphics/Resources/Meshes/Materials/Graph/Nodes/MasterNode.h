#pragma once
#include "MaterialNode.h"

namespace Bolt
{

	struct BLT_API MasterNode
	{
	public:
		ValueType Type;
		ShaderVariablePtr Variable;
		ShaderLiteralPtr DefaultValue;
	};

}