#include "Types.h"
#include "CreateScopeOp.h"
#include "../ShaderScope.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	CreateScopeOp::CreateScopeOp(ShaderScope* scope) : ShaderOp(),
		m_Scope(scope)
	{
	
	}

	void CreateScopeOp::Build(ShaderBuilder& builder) const
	{
		m_Scope->Build(builder);
	}

}