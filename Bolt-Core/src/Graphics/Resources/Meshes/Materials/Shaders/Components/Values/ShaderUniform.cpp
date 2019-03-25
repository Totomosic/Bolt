#include "Types.h"
#include "ShaderUniform.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	ShaderUniform::ShaderUniform(ValueType type) : ShaderVariable(type),
		m_DefaultValue(nullptr)
	{
	
	}

	ShaderUniform::ShaderUniform(ShaderLiteralPtr defaultValue) : ShaderUniform(defaultValue->Type())
	{
		m_DefaultValue = std::move(defaultValue);
	}

	void ShaderUniform::Build(ShaderBuilder& builder) const
	{
		if (!builder.IsDeclared(this))
		{
			m_Name = builder.GetUniformName();
			blt::string uniformLine = "uniform " + ValueTypeToGLSLString(Type()) + ' ' + m_Name;
			if (m_DefaultValue != nullptr)
			{
				uniformLine += " = " + m_DefaultValue->GetValueString();
			}
			builder.WriteUniformLine(uniformLine);
			builder.DeclareVariable(this);
		}
		builder.Write(m_Name);
	}

}