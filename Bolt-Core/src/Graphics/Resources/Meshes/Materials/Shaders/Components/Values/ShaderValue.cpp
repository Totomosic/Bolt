#include "Types.h"
#include "ShaderValue.h"

namespace Bolt
{

	ShaderValue::ShaderValue()
	{
	
	}

	ShaderValue::ShaderValue(ValueType type)
		: m_ValueType(type)
	{
	
	}

	ValueType ShaderValue::Type() const
	{
		return m_ValueType;
	}

}