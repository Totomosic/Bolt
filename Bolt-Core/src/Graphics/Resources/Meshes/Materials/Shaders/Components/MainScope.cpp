#include "Types.h"
#include "MainScope.h"
#include "ShaderBuilder.h"

namespace Bolt
{

	MainScope::MainScope(int index, ShaderScope* parent) : ShaderScope(index, parent)
	{
	
	}

	void MainScope::Build(ShaderBuilder& builder) const
	{
		builder.Indent();
		builder.Write("{\n");
		builder.SetScopeIndex(m_ScopeIndex);
		builder.Indent();
		BuildOperations(builder);
		builder.SetScopeIndex((ParentScope() == nullptr) ? 0 : ParentScope()->GetScopeIndex());
		builder.Indent();
		builder.Write("}\n");
	}

}