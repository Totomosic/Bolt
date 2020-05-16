#include "bltpch.h"
#include "SubAssignOp.h"
#include "../../ShaderBuilder.h"

namespace Bolt
{

	SubAssignOp::SubAssignOp(const ShaderLValuePtr& var, const ShaderValuePtr& value) : ShaderOp(),
		m_Variable(var), m_Value(value)
	{
		BLT_ASSERT(m_Variable->Type() == m_Value->Type() || (ValueTypeIsVector(var->Type()) && value->Type() == ValueType::Float), "Type of value must match variable");
	}

	void SubAssignOp::Build(ShaderBuilder& builder) const
	{
		m_Variable->Build(builder);
		builder.Write("-=");
		m_Value->Build(builder);
	}

}