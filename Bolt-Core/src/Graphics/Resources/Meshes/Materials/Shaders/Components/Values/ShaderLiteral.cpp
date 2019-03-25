#include "Types.h"
#include "ShaderLiteral.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	ShaderLiteral::ShaderLiteral(const blt::string& valueString, ValueType type) : ShaderValue(type),
		m_ValueString(valueString)
	{
	
	}

	const blt::string& ShaderLiteral::GetValueString() const
	{
		return m_ValueString;
	}

	void ShaderLiteral::Build(ShaderBuilder& builder) const
	{
		builder.Write(m_ValueString);
	}

}