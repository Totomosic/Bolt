#include "Types.h"
#include "GlobalScope.h"
#include "ShaderBuilder.h"

namespace Bolt
{

	GlobalScope::GlobalScope() : ShaderScope(0),
		m_Functions()
	{
	
	}

	bool GlobalScope::HasFunction(const blt::string& name) const
	{
		auto it = std::find_if(m_Functions.begin(), m_Functions.end(), [&name](const FunctionScope * scope)
			{
				return scope->GetName() == name;
			});
		return it != m_Functions.end();
	}

	const FunctionScope& GlobalScope::GetFunction(const blt::string& name) const
	{
		BLT_ASSERT(HasFunction(name), "No function exists with name {}", name);
		auto it = std::find_if(m_Functions.begin(), m_Functions.end(), [&name](const FunctionScope * scope)
			{
				return scope->GetName() == name;
			});
		return **it;
	}

	FunctionScope& GlobalScope::DefineFunction(const blt::string& name, const ValueTypeInfo& returnType, const std::vector<ValueTypeInfo>& inputs)
	{
		FunctionScope& scope = AddScope<FunctionScope>(m_ScopeIndex + 1, name, returnType, inputs);
		m_Functions.push_back(&scope);
		return scope;
	}

	void GlobalScope::Build(ShaderBuilder& builder) const
	{
		builder.SetScopeIndex(m_ScopeIndex);
		builder.NextLine();
		BuildOperations(builder);
		builder.SetScopeIndex(m_ScopeIndex);
	}

}