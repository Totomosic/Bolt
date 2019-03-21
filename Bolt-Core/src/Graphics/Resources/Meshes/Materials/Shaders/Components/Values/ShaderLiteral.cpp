#include "Types.h"
#include "ShaderLiteral.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	ShaderLiteral::ShaderLiteral(const blt::string& valueString, ValueType type) : ShaderValue(type),
		m_ValueString(valueString)
	{
	
	}

	void ShaderLiteral::Build(ShaderBuilder& builder) const
	{
		builder.Write(m_ValueString);
	}

}