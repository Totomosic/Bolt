#include "Types.h"
#include "DeclarePassInOp.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	DeclarePassInOp::DeclarePassInOp(const ShaderVariablePtr& inVar, const ShaderVariablePtr& outVar, const blt::string& meta) : ShaderOp(),
		m_InVar(inVar), m_OutVar(outVar), m_MetaData(meta)
	{
	
	}

	void DeclarePassInOp::Build(ShaderBuilder& builder) const
	{
		BLT_ASSERT(!m_OutVar->GetVarName().empty(), "Out var has not been built yet");
		if (!m_MetaData.empty())
		{
			builder.Write(m_MetaData + ' ');
		}
		builder.Write("in ");
		m_InVar->SetVarName(m_OutVar->GetVarName());
		builder.Write(ValueTypeToGLSLString(m_InVar->Type()) + ' ' + m_InVar->GetVarName());
	}

}