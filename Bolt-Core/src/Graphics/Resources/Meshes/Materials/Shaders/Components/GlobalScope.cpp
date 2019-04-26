#include "Types.h"
#include "GlobalScope.h"
#include "ShaderBuilder.h"

namespace Bolt
{

	GlobalScope::GlobalScope() : ShaderScope()
	{
	
	}

	void GlobalScope::Build(ShaderBuilder& builder) const
	{
		builder.Indent();
		builder.SetScopeIndex(m_ScopeIndex);
		BuildOperations(builder);
		builder.SetScopeIndex(m_ScopeIndex);
	}

}