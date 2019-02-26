#pragma once
#include "ShaderEnums.h"

namespace Bolt
{

	class BLT_API FunctionDecl
	{
	private:
		blt::string m_Name;
		std::vector<ValueType> m_Inputs;
		ValueType m_Output;
		ShaderType m_Shader;

	public:
		FunctionDecl();
		FunctionDecl(const blt::string& name, const std::vector<ValueType>& inputs, ValueType output, ShaderType shader);

		const blt::string& Name() const;
		const std::vector<ValueType>& InputTypes() const;
		ValueType OutputType() const;
		ShaderType Shader() const;

	};

}