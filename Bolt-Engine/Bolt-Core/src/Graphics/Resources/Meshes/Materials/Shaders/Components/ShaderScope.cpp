#include "bltpch.h"
#include "ShaderScope.h"
#include "ShaderBuilder.h"

#include "MainScope.h"
#include "Operations/CreateScopeOp.h"
#include "Operations/DeclareArrayOp.h"
#include "Operations/DeclarePassInOp.h"
#include "Operations/DeclarePassOutOp.h"
#include "Operations/SetValueOp.h"
#include "Operations/ShaderDeclareOp.h"
#include "Operations/ShaderDefineOp.h"

#include "Operations/MathOps/AddAssignOp.h"
#include "Operations/MathOps/SubAssignOp.h"
#include "Operations/MathOps/MulAssignOp.h"
#include "Operations/MathOps/DivAssignOp.h"

#include "Operations/DiscardOp.h"

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

	ShaderVariablePtr ShaderScope::DefineVar(const ShaderValuePtr& value, const blt::string& meta)
	{
		ShaderVariablePtr var = std::make_shared<ShaderVariable>(value->Type());
		AddOperation(std::make_unique<ShaderDefineOp>(var, value, meta));
		AddDeclaredVar(var.get());
		return var;
	}

	ShaderVariablePtr ShaderScope::DeclareVar(ValueType type, const blt::string& meta)
	{
		ShaderVariablePtr var = std::make_shared<ShaderVariable>(type);
		AddOperation(std::make_unique<ShaderDeclareOp>(var, meta));
		AddDeclaredVar(var.get());
		return var;
	}

	ShaderPassVariablePtr ShaderScope::DeclarePassIn(const ShaderPassVariablePtr& outVar, const blt::string& meta)
	{
		ShaderPassVariablePtr var = std::make_shared<ShaderPassVariable>(outVar->Type(), ValueTypeDim::Single, outVar->GetPassType());
		AddOperation(std::make_unique<DeclarePassInOp>(var, outVar, meta));
		AddDeclaredVar(var.get());
		return var;
	}

	ShaderPassVariablePtr ShaderScope::DeclarePassOut(ValueType type, PassType passType, const blt::string& meta)
	{
		ShaderPassVariablePtr var = std::make_shared<ShaderPassVariable>(type, ValueTypeDim::Single, passType);
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

	void ShaderScope::SetVariable(const ShaderLValuePtr& var, const ShaderValuePtr& value)
	{
		AddOperation<SetValueOp>(var, value);
	}

	void ShaderScope::AddAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value)
	{
		AddOperation<AddAssignOp>(var, value);
	}

	void ShaderScope::SubAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value)
	{
		AddOperation<SubAssignOp>(var, value);
	}

	void ShaderScope::MulAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value)
	{
		AddOperation<MulAssignOp>(var, value);
	}

	void ShaderScope::DivAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value)
	{
		AddOperation<DivAssignOp>(var, value);
	}

	void ShaderScope::Discard()
	{
		AddOperation<DiscardOp>();
	}

	void ShaderScope::Reset()
	{
		m_Operations.clear();
		m_DeclaredVariables.clear();
	}

	void ShaderScope::BuildOperations(ShaderBuilder& builder) const
	{
		for (int i = 0; i < m_Operations.size(); i++)
		{
			const auto& op = m_Operations.at(i);
			op->Build(builder);
			if (!op->IsCreateScopeOp())
			{
				builder.Write(';');
			}
			if (i != m_Operations.size() - 1)
			{
				builder.NextLine();
			}
		}
	}

	void ShaderScope::AddDeclaredVar(const ShaderVariable* var)
	{
		m_DeclaredVariables.push_back(var);
	}

	int ShaderScope::GetNextOpIndex() const
	{
		return m_Operations.size();
	}

	ShaderOp& ShaderScope::GetOpAtIndex(int index) const
	{
		BLT_ASSERT(index < m_Operations.size() && index >= 0, "Invalid index");
		return *m_Operations.at(index);
	}

}