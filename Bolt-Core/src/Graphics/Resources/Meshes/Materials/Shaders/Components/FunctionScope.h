#pragma once
#include "ShaderScope.h"
#include "Operations/ShaderReturnOp.h"

namespace Bolt
{

	class BLT_API FunctionScope : public ShaderScope
	{
	private:
		ValueTypeInfo m_ReturnType;
		std::vector<ShaderVariablePtr> m_Inputs;
		blt::string m_Name;

	public:
		FunctionScope(int scopeIndex, const blt::string& name, const ValueTypeInfo& returnType, const std::vector<ValueTypeInfo>& inputs);

		const ValueTypeInfo& GetReturnType() const;
		std::vector<ValueTypeInfo> GetInputs() const;
		const blt::string& GetName() const;

		ShaderVariablePtr GetArgument(int index) const;
		void Return(const ShaderValuePtr& value = nullptr);

		void Build(ShaderBuilder& builder) const override;
		BLT_DEFAULT_SHADERSCOPE_CLONE(FunctionScope);

	};

}