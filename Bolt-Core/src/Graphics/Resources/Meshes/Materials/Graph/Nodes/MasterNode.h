#pragma once
#include "MaterialValue.h"
#include "../../Shaders/Components/Values/ShaderVariable.h"

namespace Bolt
{

	struct BLT_API MasterNode
	{
	public:
		ShaderVariablePtr Variable;
		MaterialValue CurrentValue;

	public:
		inline ValueType GetType() const { return Variable->Type(); }
		inline bool HasValue() const { return CurrentValue.Value != nullptr; }
	};

}