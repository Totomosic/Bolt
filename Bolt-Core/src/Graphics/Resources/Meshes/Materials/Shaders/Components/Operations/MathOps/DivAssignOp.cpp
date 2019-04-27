#include "Types.h"
#include "DivAssignOp.h"
#include "../../ShaderBuilder.h"

namespace Bolt
{

	DivAssignOp::DivAssignOp(const ShaderVariablePtr& var, const ShaderValuePtr& value) : ShaderOp(),
		m_Variable(var), m_Value(value)
	{
		BLT_ASSERT(m_Variable->Type() == m_Value->Type(), "Type of value must match variable");
	}

	void DivAssignOp::Build(ShaderBuilder& builder) const
	{
		builder.Write(m_Variable->GetVarName() + " += ");
		m_Value->Build(builder);
	}

}