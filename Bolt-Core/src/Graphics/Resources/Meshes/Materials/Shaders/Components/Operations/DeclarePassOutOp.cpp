#include "Types.h"
#include "DeclarePassOutOp.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	DeclarePassOutOp::DeclarePassOutOp(const ShaderVariablePtr& outVar, const blt::string& meta) : ShaderOp(),
		m_OutVar(outVar), m_MetaData(meta)
	{

	}

	void DeclarePassOutOp::Build(ShaderBuilder& builder) const
	{
		if (!m_MetaData.empty())
		{
			builder.Write(m_MetaData + ' ');
		}
		builder.Write("out ");
		m_OutVar->m_Name = builder.GetPassName();
		builder.Write(ValueTypeToGLSLString(m_OutVar->Type()) + ' ' + m_OutVar->GetVarName());
	}

}