#include "Types.h"
#include "FragColorAttribute.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	FragColorAttribute::FragColorAttribute(ShaderValuePtr value) : ShaderAttribute(ShaderAttribute::FRAG_COLOR, std::move(value))
	{
	
	}

	void FragColorAttribute::Build(ShaderBuilder& builder) const
	{
		if (!builder.IsDeclared(this))
		{
			builder.WriteStreamLine("layout(location = 0) out vec4 " + GetName());
			builder.Write('\t' + GetName() + " = ");
			m_Value->Build(builder);
			builder.Write(";\n");
		}
	}

}