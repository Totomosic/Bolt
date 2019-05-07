#include "Types.h"
#include "ForLoopScope.h"
#include "ShaderBuilder.h"

namespace Bolt
{

	ForLoopScope::ForLoopScope(int scopeIndex, std::unique_ptr<ShaderOp>&& init, const ShaderValuePtr& condition, std::unique_ptr<ShaderOp>&& iteration) : ShaderScope(scopeIndex),
		m_InitOp(std::move(init)), m_Condition(condition), m_IterationOp(std::move(iteration))
	{
	
	}

	void ForLoopScope::Build(ShaderBuilder& builder) const
	{
		builder.Write("for (");
		m_InitOp->Build(builder);
		builder.Write("; ");
		m_Condition->Build(builder);
		builder.Write("; ");
		m_IterationOp->Build(builder);
		builder.Write(')');
		builder.NextLine();
		builder.Write('{');
		builder.SetScopeIndex(m_ScopeIndex);
		builder.NextLine();
		BuildOperations(builder);
		builder.SetScopeIndex(m_ScopeIndex - 1);
		builder.NextLine();
		builder.Write('}');
	}

	std::unique_ptr<ShaderScope> ForLoopScope::Clone() const
	{
		return std::make_unique<ForLoopScope>(m_ScopeIndex, m_InitOp->Clone(), m_Condition, m_IterationOp->Clone());
	}

}