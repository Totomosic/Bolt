#include "Types.h"
#include "ShaderLiteral.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	void ShaderLiteral::Build(ShaderBuilder& builder) const
	{
		builder.Write(m_ValueString);
	}

}