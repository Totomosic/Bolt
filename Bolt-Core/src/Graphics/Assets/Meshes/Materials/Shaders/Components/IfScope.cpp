#include "bltpch.h"
#include "IfScope.h"
#include "ShaderBuilder.h"

namespace Bolt
{

	IfScope::IfScope(int scopeIndex, const ShaderValuePtr& condition) : ShaderScope(scopeIndex),
		m_Condition(condition)
	{

	}

	void IfScope::Build(ShaderBuilder& builder) const
	{
		builder.Write("if(");
		m_Condition->Build(builder);
		builder.Write(")");
		builder.NextLine();
		builder.Write("{");
		builder.SetScopeIndex(m_ScopeIndex);
		builder.NextLine();
		BuildOperations(builder);
		builder.SetScopeIndex(m_ScopeIndex - 1);
		builder.NextLine();
		builder.Write("}");
	}

	std::unique_ptr<ShaderScope> IfScope::Clone() const
	{
		return std::make_unique<IfScope>(m_ScopeIndex, m_Condition);
	}

}