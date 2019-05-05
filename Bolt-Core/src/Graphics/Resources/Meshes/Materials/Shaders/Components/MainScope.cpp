#include "Types.h"
#include "MainScope.h"
#include "ShaderBuilder.h"

namespace Bolt
{

	MainScope::MainScope(int index) : ShaderScope(index)
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
		builder.SetScopeIndex(m_ScopeIndex - 1);
		builder.NextLine();
		builder.Write('}');
	}

}