#include "Types.h"
#include "ShaderVariable.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	ShaderVariable::ShaderVariable(ValueType type) : ShaderValue(type),
		m_Value(nullptr), m_ShaderType(ShaderType::Ignore)
	{
	
	}

	ShaderVariable::ShaderVariable(ShaderValuePtr value) : ShaderValue(value->Type()),
		m_Value(std::move(value)), m_ShaderType(ShaderType::Ignore)
	{
	
	}
	
	ShaderType ShaderVariable::GetShaderType() const
	{
		return m_ShaderType;
	}
	
	void ShaderVariable::Build(ShaderBuilder& builder) const
	{
		if (!builder.IsDeclared(this))
		{
			BLT_ASSERT(m_Value != nullptr, "No value found");
			int cursor = builder.SaveLineCursor();
			m_Name = builder.GetVariableName();
			builder.PreviousLine();
			builder.Write('\t' + ValueTypeToGLSLString(Type()) + ' ' + m_Name + " = ");
			m_Value->Build(builder);
			builder.Write(";\n");
			builder.DeclareVariable(this);
			builder.LoadLineCursor(cursor);
		}
		builder.Write(m_Name);
	}

	ShaderVariablePtr ShaderVariable::Create(ShaderValuePtr value)
	{
		return std::make_shared<ShaderVariable>(std::move(value));
	}

}