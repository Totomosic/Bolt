#include "Types.h"
#include "ShaderDefineOp.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	ShaderDefineOp::ShaderDefineOp(const ShaderVariablePtr& var) : ShaderOp(),
		m_Variable(var)
	{
	
	}

	void ShaderDefineOp::Build(ShaderBuilder& builder) const
	{
	
	}

}