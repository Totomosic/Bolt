#include "Types.h"
#include "CreateScopeOp.h"
#include "../ShaderScope.h"
#include "../ShaderBuilder.h"

namespace Bolt
{

	CreateScopeOp::CreateScopeOp(std::unique_ptr<ShaderScope>&& scope) : ShaderOp(),
		m_Scope(std::move(scope))
	{
	
	}

	void CreateScopeOp::Build(ShaderBuilder& builder) const
	{
		m_Scope->Build(builder);
	}

	std::unique_ptr<ShaderOp> CreateScopeOp::Clone() const
	{
		return std::make_unique<CreateScopeOp>(m_Scope->Clone());
	}

}