#include "bltpch.h"
#include "GlobalScope.h"
#include "ShaderBuilder.h"

namespace Bolt
{

	GlobalScope::GlobalScope() : ShaderScope(0),
		m_FunctionIndices()
	{
	
	}

	bool GlobalScope::HasFunction(const blt::string& name) const
	{
		auto it = std::find_if(m_FunctionIndices.begin(), m_FunctionIndices.end(), [this, &name](int opIndex)
			{
				return GetFunctionByIndex(opIndex).GetName() == name;
			});
		return it != m_FunctionIndices.end();
	}

	FunctionScope& GlobalScope::GetFunction(const blt::string& name) const
	{
		BLT_ASSERT(HasFunction(name), "No function exists with name {}", name);
		auto it = std::find_if(m_FunctionIndices.begin(), m_FunctionIndices.end(), [this, &name](int opIndex)
			{
				return GetFunctionByIndex(opIndex).GetName() == name;
			});
		return GetFunctionByIndex(*it);
	}

	FunctionScope& GlobalScope::DefineFunction(const blt::string& name, const ValueTypeInfo& returnType, const std::vector<ValueTypeInfo>& inputs)
	{
		int index = GetNextOpIndex();
		FunctionScope& scope = AddScope<FunctionScope>(m_ScopeIndex + 1, name, returnType, inputs);
		m_FunctionIndices.push_back(index);
		return scope;
	}

	void GlobalScope::Build(ShaderBuilder& builder) const
	{
		builder.SetScopeIndex(m_ScopeIndex);
		builder.NextLine();
		BuildOperations(builder);
		builder.SetScopeIndex(m_ScopeIndex);
		builder.NextLine();
	}

	void GlobalScope::Reset()
	{
		ShaderScope::Reset();
		m_FunctionIndices.clear();
	}

	FunctionScope& GlobalScope::GetFunctionByIndex(int index) const
	{
		return (FunctionScope&)GetOpAtIndex(index);
	}

}