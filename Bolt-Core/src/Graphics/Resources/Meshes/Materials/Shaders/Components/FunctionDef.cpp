#include "Types.h"
#include "FunctionDef.h"

namespace Bolt
{

	FunctionDef::FunctionDef()
	{
		
	}

	FunctionDef::FunctionDef(const FunctionDecl& declaration)
		: m_Declaration(declaration), m_Params(), m_FunctionResults(), m_OutputValue(nullptr)
	{
		int count = 0;
		for (ValueType input : m_Declaration.InputTypes())
		{
			m_Params.push_back(std::make_shared<ShaderValue>(OutputValueType::FunctionParam, input, declaration.Shader(), GetParamName(count++)));
		}
	}

	const FunctionDecl& FunctionDef::Declaration() const
	{
		return m_Declaration;
	}

	ShaderValuePtr FunctionDef::GetParam(int index)
	{
		BLT_ASSERT(index < m_Params.size(), "Param out of range");
		return m_Params.at(index);
	}

	void FunctionDef::Output(ShaderValuePtr value)
	{
		BLT_ASSERT(value->Type() == m_Declaration.OutputType(), "Type mismatch");
		m_OutputValue = value;
	}

	ShaderValuePtr FunctionDef::FuncResult(const FunctionDecl& declaration, const std::vector<ShaderValuePtr>& inputs)
	{
		BLT_ASSERT(declaration.Shader() == m_Declaration.Shader(), "Shader mismatch");
		ShaderValuePtr value = std::make_shared<ShaderValue>(OutputValueType::Value, declaration.OutputType(), m_Declaration.Shader(), GetNextFuncResultName());
		blt::string source = GetAssignmentString(declaration, inputs);
		m_FunctionResults.push_back({ source, value });
		return value;
	}

	ShaderValuePtr FunctionDef::FuncResult(const BuiltInFunc& declaration, const std::vector<ShaderValuePtr>& inputs)
	{
		BuiltInFunc::OverloadInstance source = declaration.GetSource(inputs);
		ShaderValuePtr value = std::make_shared<ShaderValue>(OutputValueType::Value, source.Output, m_Declaration.Shader(), GetNextFuncResultName());
		m_FunctionResults.push_back({ source.Source, value });
		return value;
	}

	blt::string FunctionDef::GetSource() const
	{
		BLT_ASSERT(m_Declaration.OutputType() == ValueType::Void || m_OutputValue != nullptr, "No given return value");
		blt::string source = GetTypeString(m_Declaration.OutputType()) + ' ' + m_Declaration.Name() + '(';
		if (m_Params.size() > 0)
		{
			ShaderValuePtr p0 = m_Params.at(0);
			source += GetTypeString(p0->Type()) + ' ' + p0->VarName();
			for (int i = 1; i < m_Params.size(); i++)
			{
				ShaderValuePtr p = m_Params.at(i);
				source += ", " + GetTypeString(p->Type()) + ' ' + p->VarName();
			}
		}
		source += ")\n{\n";
		for (const FunctionValueInfo& result : m_FunctionResults)
		{
			source += '\t' + GetTypeString(result.Value->Type()) + ' ' + result.Value->VarName() + " = " + result.FuncString + ";\n";
		}
		if (m_Declaration.OutputType() != ValueType::Void)
		{
			source += "\treturn " + m_OutputValue->VarName() + ";\n";
		}
		source += '}';
		return source;
	}

	blt::string FunctionDef::GetParamName(int index) const
	{
		return "Param_" + std::to_string(index);
	}

	blt::string FunctionDef::GetNextFuncResultName() const
	{
		return "FuncResult_" + std::to_string(m_FunctionResults.size());
	}

	blt::string FunctionDef::GetTypeString(ValueType type)
	{
		switch (type)
		{
		case ValueType::Void:
			return "void";
		case ValueType::Int:
			return "int";
		case ValueType::Float:
			return "float";
		case ValueType::Vector2i:
			return "vec2i";
		case ValueType::Vector3i:
			return "vec3i";
		case ValueType::Vector4i:
			return "vec4i";
		case ValueType::Vector2f:
			return "vec2";
		case ValueType::Vector3f:
			return "vec3";
		case ValueType::Vector4f:
			return "vec4";
		case ValueType::Matrix2f:
			return "mat2";
		case ValueType::Matrix3f:
			return "mat3";
		case ValueType::Matrix4f:
			return "mat4";
		case ValueType::Texture1D:
			return "sampler1D";
		case ValueType::Texture2D:
			return "sampler2D";
		case ValueType::Texture3D:
			return "sampler3D";
		case ValueType::TextureCube:
			return "samplerCube";
		}
		BLT_ASSERT(false, "Cannot get type string");
		return "";
	}
	
	blt::string FunctionDef::GetAssignmentString(const FunctionDecl& declaration, const std::vector<ShaderValuePtr>& inputs)
	{
		blt::string source = declaration.Name() + '(';
		if (inputs.size() > 0)
		{
			ShaderValuePtr p0 = inputs.at(0);
			source += p0->VarName();
			for (int i = 1; i < inputs.size(); i++)
			{
				ShaderValuePtr p = inputs.at(i);
				source += ", " + p->VarName();
			}
		}
		source += ')';
		return source;
	}

	blt::string FunctionDef::GetAssignmentString(const BuiltInFunc& declaration, const std::vector<ShaderValuePtr>& inputs)
	{
		BuiltInFunc::OverloadInstance source = declaration.GetSource(inputs);
		return source.Source;
	}

}