#include "bltpch.h"
#include "ShaderVariable.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	// =======================================================================================================================================
	// SHADER VARIABLE
	// =======================================================================================================================================

	ShaderVariable::ShaderVariable(ValueType type, ValueTypeDim dim) : ShaderLValue(type, dim)
	{
	
	}

	const std::string& ShaderVariable::GetVarName() const
	{
		return m_Name;
	}

	void ShaderVariable::SetVarName(const std::string& varName)
	{ 
		m_Name = varName;
	}
	
	void ShaderVariable::Build(ShaderBuilder& builder) const
	{
		builder.Write(m_Name);
	}

	// =======================================================================================================================================
	// SHADER PASS VARIABLE
	// =======================================================================================================================================

	ShaderPassVariable::ShaderPassVariable(ValueType type, ValueTypeDim dim, PassType passType) : ShaderVariable(type, dim),
		m_PassType(passType)
	{
	
	}

	PassType ShaderPassVariable::GetPassType() const
	{
		return m_PassType;
	}

}