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
		builder.Write("void main()");
		builder.NextLine();
		builder.Write('{');
		builder.SetScopeIndex(m_ScopeIndex);
		builder.NextLine();
		BuildOperations(builder);
		builder.SetScopeIndex((ParentScope() == nullptr) ? 0 : ParentScope()->GetScopeIndex());
		builder.NextLine();
		builder.Write('}');
	}

}