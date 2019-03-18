#pragma once
#include "ShaderValue.h"

namespace Bolt
{

	class ShaderVariable;
	using ShaderVariablePtr = std::shared_ptr<ShaderVariable>;

	class BLT_API ShaderVariable : public ShaderValue
	{
	public:
		ShaderVariable();
		ShaderVariable(ValueType type);
	};

}