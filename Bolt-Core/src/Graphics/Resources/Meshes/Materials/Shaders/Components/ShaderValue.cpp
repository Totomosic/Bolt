#include "Types.h"
#include "ShaderValue.h"

namespace Bolt
{

	ShaderValue::ShaderValue()
	{
	
	}

	ShaderValue::ShaderValue(OutputValueType outputType, ValueType valueType, ShaderType shader, const blt::string& valueName)
		: m_Type(outputType), m_ValueType(valueType), m_ShaderType(shader), m_ValueName(valueName)
	{
	
	}

	OutputValueType ShaderValue::OutputType() const
	{
		return m_Type;
	}

	ValueType ShaderValue::Type() const
	{
		return m_ValueType;
	}

	ShaderType ShaderValue::Shader() const
	{
		return m_ShaderType;
	}

	blt::string ShaderValue::VarName() const
	{
		return m_ValueName;
	}

	void ShaderValue::SetShader(ShaderType shader)
	{
		m_ShaderType = shader;
	}

}