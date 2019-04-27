#include "Types.h"
#include "DeclareArrayOp.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	DeclareArrayOp::DeclareArrayOp(const ShaderVariablePtr& arr, const ShaderLiteralPtr& length, const blt::string& meta) : ShaderOp(),
		m_Variable(arr), m_Length(length), m_MetaData(meta)
	{
		BLT_ASSERT(arr->TypeDimension() == ValueTypeDim::Array, "Input 0 must be an array");
		BLT_ASSERT(length->Type() == ValueType::Int, "Length must be an int");
	}

	void DeclareArrayOp::Build(ShaderBuilder& builder) const
	{
		if (!m_MetaData.empty())
		{
			builder.Write(m_MetaData + ' ');
		}
		m_Variable->m_Name = builder.GetVariableName();
		builder.Write(ValueTypeToGLSLString(m_Variable->Type()) + ' ' + m_Variable->GetVarName() + '[');
		m_Length->Build(builder);
		builder.Write(']');
	}

}