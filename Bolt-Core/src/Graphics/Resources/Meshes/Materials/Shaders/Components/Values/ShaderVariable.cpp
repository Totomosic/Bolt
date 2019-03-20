#include "Types.h"
#include "ShaderVariable.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	ShaderVariable::ShaderVariable(ValueType type) : ShaderValue(type),
		m_Value(nullptr)
	{
	
	}

	ShaderVariable::ShaderVariable(ShaderValuePtr value) : ShaderValue(value->Type()),
		m_Value(std::move(value))
	{
	
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

}