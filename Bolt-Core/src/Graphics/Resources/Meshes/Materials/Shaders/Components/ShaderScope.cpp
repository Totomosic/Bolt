#include "Types.h"
#include "ShaderScope.h"
#include "ShaderBuilder.h"

#include "MainScope.h"

namespace Bolt
{

	ShaderScope::ShaderScope() : ShaderScope(0)
	{
	
	}

	ShaderScope::ShaderScope(int index)
		: m_ScopeIndex(index), m_DeclaredVariables(), m_Operations()
	{
		
	}

	ShaderScope::ShaderScope(const ShaderScope& other)
		: m_ScopeIndex(other.m_ScopeIndex), m_DeclaredVariables(other.m_DeclaredVariables), m_Operations()
	{
		m_Operations.reserve(other.m_Operations.size());
		for (const auto& op : other.m_Operations)
		{
			m_Operations.push_back(op->Clone());
		}
	}

	ShaderScope& ShaderScope::operator=(const ShaderScope& other)
	{
		m_Operations.clear();
		m_Operations.reserve(other.m_Operations.size());		
		for (const auto& op : other.m_Operations)
		{
			m_Operations.push_back(op->Clone());
		}
		m_DeclaredVariables = other.m_DeclaredVariables;
		m_ScopeIndex = other.m_ScopeIndex;
		return *this;
	}

	int ShaderScope::GetScopeIndex() const
	{
		return m_ScopeIndex;
	}

	bool ShaderScope::IsDefinedInThisScope(const ShaderVariable* var) const
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

	ShaderVariablePtr ShaderScope::DeclareArray(ValueType type, const ShaderLiteralPtr& length, const blt::string& meta)
	{
		ShaderVariablePtr var = std::make_shared<ShaderVariable>(type, ValueTypeDim::Array);
		AddOperation(std::make_unique<DeclareArrayOp>(var, length, meta));
		AddDeclaredVar(var.get());
		return var;
	}

	void ShaderScope::AddOperation(std::unique_ptr<ShaderOp>&& op)
	{
		m_Operations.push_back(std::move(op));
	}

	void ShaderScope::InsertOperation(int index, std::unique_ptr<ShaderOp>&& op)
	{
		m_Operations.insert(m_Operations.begin() + index, std::move(op));
	}

	ShaderScope& ShaderScope::AddScope(std::unique_ptr<ShaderScope>&& scope)
	{
		ShaderScope* ptr = scope.get();
		AddOperation(std::make_unique<CreateScopeOp>(std::move(scope)));
		return *ptr;
	}

	void ShaderScope::BuildOperations(ShaderBuilder& builder) const
	{
		for (const auto& op : m_Operations)
		{
			op->Build(builder);
			builder.Write(';');
			builder.NextLine();
		}
	}

	void ShaderScope::AddDeclaredVar(const ShaderVariable* var)
	{
		m_DeclaredVariables.push_back(var);
	}

}