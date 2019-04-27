#include "Types.h"
#include "ShaderVariable.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	ShaderVariable::ShaderVariable(ValueType type, ValueTypeDim dim) : ShaderValue(type, dim),
		m_ShaderType(ShaderType::Ignore)
	{
	
	}
	
	ShaderType ShaderVariable::GetShaderType() const
	{
		return m_ShaderType;
	}

	const blt::string& ShaderVariable::GetVarName() const
	{
		return m_Name;
	}
	
	void ShaderVariable::Build(ShaderBuilder& builder) const
	{
		builder.Write(m_Name);
	}

}