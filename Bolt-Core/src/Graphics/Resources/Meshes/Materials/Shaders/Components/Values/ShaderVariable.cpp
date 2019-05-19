#include "bltpch.h"
#include "ShaderVariable.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	ShaderVariable::ShaderVariable(ValueType type, ValueTypeDim dim) : ShaderLValue(type, dim)
	{
	
	}

	const blt::string& ShaderVariable::GetVarName() const
	{
		return m_Name;
	}

	void ShaderVariable::SetVarName(const blt::string& varName)
	{ 
		m_Name = varName;
	}
	
	void ShaderVariable::Build(ShaderBuilder& builder) const
	{
		builder.Write(m_Name);
	}

}