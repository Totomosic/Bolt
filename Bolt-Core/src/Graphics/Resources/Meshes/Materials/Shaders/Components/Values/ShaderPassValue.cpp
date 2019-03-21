#include "Types.h"
#include "ShaderPassValue.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	ShaderPassValue::ShaderPassValue(ShaderValuePtr value) : ShaderVariable(std::move(value))
	{
	
	}

	void ShaderPassValue::Build(ShaderBuilder& builder) const
	{
		if (!builder.IsDeclared(this))
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

}