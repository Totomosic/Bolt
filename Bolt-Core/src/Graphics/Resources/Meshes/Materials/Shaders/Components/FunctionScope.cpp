#include "bltpch.h"
#include "FunctionScope.h"
#include "ShaderBuilder.h"

namespace Bolt
{

	FunctionScope::FunctionScope(int scopeIndex, const blt::string& name, const ValueTypeInfo& returnType, const std::vector<ValueTypeInfo>& inputs) : ShaderScope(scopeIndex),
		m_ReturnType(returnType), m_Inputs(), m_Name(name)
	{
		for (int i = 0; i < inputs.size(); i++)
		{
			const auto& input = inputs.at(i);
			ShaderVariablePtr var = std::make_shared<ShaderVariable>(input.Type, input.Dimension);
			var->SetVarName("arg_" + std::to_string(i));
			m_Inputs.push_back(std::move(var));
		}
	}

	const ValueTypeInfo& FunctionScope::GetReturnType() const
	{
		return m_ReturnType;
	}

	std::vector<ValueTypeInfo> FunctionScope::GetInputs() const
	{
		std::vector<ValueTypeInfo> result;
		result.reserve(m_Inputs.size());
		for (const auto& input : m_Inputs)
		{
			result.push_back({ input->Type(), input->TypeDimension() });
		}
		return result;
	}

	const blt::string& FunctionScope::GetName() const
	{
		return m_Name;
	}

	ShaderVariablePtr FunctionScope::GetArgument(int index) const
	{
		BLT_ASSERT(index < m_Inputs.size(), "Argument does not exist with index {}", index);
		return m_Inputs.at(index);
	}

	void FunctionScope::Return(const ShaderValuePtr& value)
	{
		BLT_ASSERT((m_ReturnType.Type == ValueType::Void && value == nullptr) || (m_ReturnType.Type != ValueType::Void && value != nullptr && value->Type() == m_ReturnType.Type), "Cannot return value");
		AddOperation<ShaderReturnOp>(value);
	}

	void FunctionScope::Build(ShaderBuilder& builder) const
	{
		builder.Write(ValueTypeToGLSLString(m_ReturnType.Type) + ' ' + m_Name + '(');
		if (m_Inputs.size() > 0)
		{
			const auto& arg = m_Inputs.at(0);
			builder.Write(ValueTypeToGLSLString(arg->Type()) + ' ' + arg->GetVarName());
			for (int i = 1; i < m_Inputs.size(); i++)
			{
				const auto& arg = m_Inputs.at(i);
				builder.Write(", " + ValueTypeToGLSLString(arg->Type()) + ' ' + arg->GetVarName());
			}
		}
		builder.Write(')');
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