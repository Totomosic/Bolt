#include "Types.h"
#include "ShaderArrayValue.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	ShaderArrayValue::ShaderArrayValue(const ShaderLValuePtr& arr, const ShaderValuePtr& index, ValueType overrideType) : ShaderLValue((overrideType == ValueType::Void) ? arr->Type() : overrideType, ValueTypeDim::Single),
		m_Array(arr), m_Index(index)
	{
	
	}

	void ShaderArrayValue::Build(ShaderBuilder& builder) const
	{
		m_Array->Build(builder);
		builder.Write('[');
		m_Index->Build(builder);
		builder.Write(']');
	}

}