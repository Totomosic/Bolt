#include "Types.h"
#include "SetValueOp.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	SetValueOp::SetValueOp(const ShaderVariablePtr& var, const ShaderValuePtr& value) : ShaderOp(),
		m_Variable(var), m_Value(value)
	{
	
	}

	void SetValueOp::Build(ShaderBuilder& builder) const
	{
		builder.Write(m_Variable->GetVarName() + " = ");
		m_Value->Build(builder);
		builder.Write(';');
	}

}