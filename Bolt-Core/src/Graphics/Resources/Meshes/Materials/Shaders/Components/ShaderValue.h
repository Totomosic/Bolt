#pragma once
#include "ShaderEnums.h"

namespace Bolt
{
	class BLT_API ShaderValue
	{
	private:
		OutputValueType m_Type;
		ValueType m_ValueType;
		ShaderType m_ShaderType;
		blt::string m_ValueName;

	public:
		ShaderValue();
		ShaderValue(OutputValueType type, ValueType valueType, ShaderType shader, const blt::string& valueName);

		OutputValueType OutputType() const;
		ValueType Type() const;
		ShaderType Shader() const;
		blt::string VarName() const;

		friend class ShaderProgramFactory;

	private:
		void SetShader(ShaderType shader);

	};

	using ShaderValuePtr = std::shared_ptr<ShaderValue>;

}