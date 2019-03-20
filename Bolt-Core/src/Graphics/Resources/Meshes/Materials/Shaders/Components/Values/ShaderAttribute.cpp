#include "Types.h"
#include "ShaderAttribute.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	ShaderAttribute::ShaderAttribute(int attribute, ShaderValuePtr value) : ShaderVariable(std::move(value)),
		m_Attribute(attribute)
	{
	
	}

	void ShaderAttribute::Build(ShaderBuilder& builder) const
	{
		if (!builder.IsDeclared(this))
		{
			builder.Write('\t' + GetName() + " = ");
			m_Value->Build(builder);
			builder.Write(";\n");
		}
	}

	blt::string ShaderAttribute::GetName() const
	{
		switch (m_Attribute)
		{
		case VERTEX_POSITION:
			return "gl_Position";
		case FRAG_COLOR:
			return "FragColor";
		}
		BLT_ASSERT(false, "Cannot find name");
		return "";
	}

}