#include "Types.h"
#include "ShaderReturnOp.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	ShaderReturnOp::ShaderReturnOp(const ShaderValuePtr& value) : ShaderOp(),
		m_Value(value)
	{
	
	}

	void ShaderReturnOp::Build(ShaderBuilder& builder) const
	{
		builder.Write("return");
		if (m_Value != nullptr)
		{
			builder.Write(' ');
			m_Value->Build(builder);
		}
	}

}