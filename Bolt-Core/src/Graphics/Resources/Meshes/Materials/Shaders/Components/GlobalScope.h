#pragma once
#include "ShaderScope.h"
#include "FunctionScope.h"

namespace Bolt
{

	class BLT_API GlobalScope : public ShaderScope
	{
	private:
		std::vector<int> m_FunctionIndices;

	public:
		GlobalScope();

		bool HasFunction(const blt::string& name) const;
		FunctionScope& GetFunction(const blt::string& name) const;
		FunctionScope& DefineFunction(const blt::string& name, const ValueTypeInfo& returnType, const std::vector<ValueTypeInfo>& inputs);

		void Build(ShaderBuilder& builder) const override;
		BLT_DEFAULT_SHADERSCOPE_CLONE(GlobalScope);
		void Reset() override;

	private:
		FunctionScope& GetFunctionByIndex(int index) const;

	};

}