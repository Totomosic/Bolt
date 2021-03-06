#include "bltpch.h"
#include "DeclarePassOutOp.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	DeclarePassOutOp::DeclarePassOutOp(const ShaderVariablePtr& outVar, const std::string& meta) : ShaderOp(),
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
		m_OutVar->SetVarName(builder.GetPassName());
		builder.Write(ValueTypeToGLSLString(m_OutVar->Type()) + ' ' + m_OutVar->GetVarName());
	}

}