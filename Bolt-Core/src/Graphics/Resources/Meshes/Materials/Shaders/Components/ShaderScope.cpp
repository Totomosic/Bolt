#include "Types.h"
#include "ShaderScope.h"
#include "ShaderBuilder.h"

#include "MainScope.h"

namespace Bolt
{

	ShaderScope::ShaderScope() : ShaderScope(0, nullptr)
	{
	
	}

	ShaderScope::ShaderScope(int index, ShaderScope* parent)
		: m_ChildScopes(), m_ParentScope(parent), m_ScopeIndex(index), m_DeclaredVariables(), m_Operations()
	{
	
	}

	ShaderScope* ShaderScope::ParentScope() const
	{
		return m_ParentScope;
	}

	int ShaderScope::GetScopeIndex() const
	{
		return m_ScopeIndex;
	}

	bool ShaderScope::IsDefinedInThisScope(const ShaderVariable* var) const
	{
		return true;
	}

	bool ShaderScope::IsDefined(const ShaderVariable* var) const
	{
		return true;
	}

	ShaderVariablePtr ShaderScope::DefineVariable(const ShaderValuePtr& value, const blt::string& meta)
	{
		ShaderVariablePtr var = std::make_shared<ShaderVariable>(value->Type());
		AddOperation(std::make_unique<ShaderDefineOp>(var, value, meta));
		AddDeclaredVar(var.get());
		return var;
	}

	ShaderVariablePtr ShaderScope::DeclareVariable(ValueType type, const blt::string& meta)
	{
		ShaderVariablePtr var = std::make_shared<ShaderVariable>(type);
		AddOperation(std::make_unique<ShaderDeclareOp>(var, meta));
		AddDeclaredVar(var.get());
		return var;
	}

	ShaderVariablePtr ShaderScope::DeclarePassIn(const ShaderVariablePtr& outVar, const blt::string& meta)
	{
		ShaderVariablePtr var = std::make_shared<ShaderVariable>(outVar->Type());
		AddOperation(std::make_unique<DeclarePassInOp>(var, outVar, meta));
		AddDeclaredVar(var.get());
		return var;
	}

	ShaderVariablePtr ShaderScope::DeclarePassOut(ValueType type, const blt::string& meta)
	{
		ShaderVariablePtr var = std::make_shared<ShaderVariable>(type);
		AddOperation(std::make_unique<DeclarePassOutOp>(var, meta));
		AddDeclaredVar(var.get());
		return var;
	}

	void ShaderScope::AddOperation(std::unique_ptr<ShaderOp>&& op)
	{
		m_Operations.push_back(std::move(op));
	}

	ShaderScope& ShaderScope::AddMainScope()
	{
		std::unique_ptr<MainScope> main = std::make_unique<MainScope>(m_ScopeIndex + 1, this);
		MainScope* ptr = main.get();
		AddChildScope(std::move(main));		
		return *ptr;
	}

	void ShaderScope::AddChildScope(std::unique_ptr<ShaderScope>&& scope)
	{
		AddOperation(std::make_unique<CreateScopeOp>(scope.get()));
		m_ChildScopes.push_back(std::move(scope));		
	}

	void ShaderScope::BuildOperations(ShaderBuilder& builder) const
	{
		for (const auto& op : m_Operations)
		{
			op->Build(builder);
			builder.NextLine();
		}
	}

	void ShaderScope::AddDeclaredVar(const ShaderVariable* var)
	{
		m_DeclaredVariables.push_back(var);
	}

}