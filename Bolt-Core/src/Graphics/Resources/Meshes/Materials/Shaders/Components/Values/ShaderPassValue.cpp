#include "Types.h"
#include "ShaderPassValue.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	ShaderPassValue::ShaderPassValue(ShaderValuePtr value, ShaderType passTo) : ShaderVariable(std::move(value)),
		m_PassTo(passTo)
	{
	
	}

	void ShaderPassValue::Build(ShaderBuilder& builder) const
	{
		if (!builder.IsDeclared(this))
		{
			if (builder.GetShaderType() == m_PassTo)
			{
				BLT_ASSERT(!m_Name.empty(), "Pass variable has not been built prior, name is empty");
				builder.WritePassLine("in " + ValueTypeToGLSLString(Type()) + ' ' + m_Name);
				builder.DeclareVariable(this);
			}
			else
			{
				BLT_ASSERT(m_Value != nullptr, "No value found");
				m_Name = builder.GetPassName();
				builder.WritePassLine("out " + ValueTypeToGLSLString(Type()) + ' ' + m_Name);
				builder.Write('\t' + m_Name + " = ");
				m_Value->Build(builder);
				builder.Write(";\n");
				builder.DeclareVariable(this);
			}
		}
		if (builder.GetShaderType() == m_PassTo)
		{
			builder.Write(m_Name);
		}
	}

}