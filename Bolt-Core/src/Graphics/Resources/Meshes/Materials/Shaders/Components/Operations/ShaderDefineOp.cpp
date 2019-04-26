#include "Types.h"
#include "ShaderDefineOp.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	ShaderDefineOp::ShaderDefineOp(const ShaderVariablePtr& var, const ShaderValuePtr& value, const blt::string& meta) : ShaderOp(),
		m_Variable(var), m_Value(value), m_MetaData(meta)
	{
	
	}

	void ShaderDefineOp::Build(ShaderBuilder& builder) const
	{
		builder.Indent();
		if (!m_MetaData.empty())
		{
			builder.Write(m_MetaData + ' ');
		}
		m_Variable->m_Name = builder.GetVariableName();
		builder.Write(ValueTypeToGLSLString(m_Variable->Type()) + ' ' + m_Variable->GetVarName() + " = ");
		m_Value->Build(builder);
		builder.Write(";\n");
	}

}