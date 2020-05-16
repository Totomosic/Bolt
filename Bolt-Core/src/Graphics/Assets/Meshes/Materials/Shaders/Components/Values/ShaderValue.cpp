#include "bltpch.h"
#include "ShaderValue.h"

namespace Bolt
{

	ShaderValue::ShaderValue()
		: m_ValueType(), m_Dimension(ValueTypeDim::Single)
	{
	
	}

	ShaderValue::ShaderValue(ValueType type, ValueTypeDim dim)
		: m_ValueType(type), m_Dimension(dim)
	{
	
	}

	ValueType ShaderValue::Type() const
	{
		return m_ValueType;
	}

	ValueTypeDim ShaderValue::TypeDimension() const
	{
		return m_Dimension;
	}

}