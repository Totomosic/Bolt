#include "Types.h"
#include "ShaderDeclareOp.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	ShaderDeclareOp::ShaderDeclareOp(const ShaderVariablePtr& var, const blt::string& meta) : ShaderOp(),
		m_Variable(var), m_MetaData(meta)
	{

	}

	void ShaderDeclareOp::Build(ShaderBuilder& builder) const
	{
		if (!m_MetaData.empty())
		{
			builder.Write(m_MetaData + ' ');
		}
		m_Variable->m_Name = builder.GetVariableName();
		builder.Write(ValueTypeToGLSLString(m_Variable->Type()) + ' ' + m_Variable->GetVarName() + ';');
	}

}