#include "bltpch.h"
#include "MulAssignOp.h"
#include "../../ShaderBuilder.h"

namespace Bolt
{

	MulAssignOp::MulAssignOp(const ShaderLValuePtr& var, const ShaderValuePtr& value) : ShaderOp(),
		m_Variable(var), m_Value(value)
	{
		BLT_ASSERT(m_Variable->Type() == m_Value->Type(), "Type of value must match variable");
	}

	void MulAssignOp::Build(ShaderBuilder& builder) const
	{
		m_Variable->Build(builder);
		builder.Write(" *= ");
		m_Value->Build(builder);
	}

}