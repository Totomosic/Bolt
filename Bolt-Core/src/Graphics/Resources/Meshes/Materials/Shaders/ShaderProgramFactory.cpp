#include "Types.h"
#include "ShaderProgramFactory.h"

namespace Bolt
{

	ShaderProgramFactory::ShaderProgramFactory(ShaderType shader)
		: m_CurrentShader(shader), m_Context(m_CurrentShader), m_StreamValues(), m_Uniforms(), m_PassValues(), m_PassedToMe(), m_Values()
	{
	
	}

	const FunctionContext& ShaderProgramFactory::Context() const
	{
		return m_Context;
	}

	FunctionContext& ShaderProgramFactory::Context()
	{
		return m_Context;
	}

	ValueType ShaderProgramFactory::GetValueTypeOfStream(ShaderStream stream) const
	{
		switch (stream)
		{
		case ShaderStream::Position:
			return ValueType::Vector4f;
		case ShaderStream::Normal:
			return ValueType::Vector3f;
		case ShaderStream::TexCoord:
			return ValueType::Vector2f;
		case ShaderStream::Color:
			return ValueType::Vector4f;
		case ShaderStream::Tangent:
			return ValueType::Vector3f;
		}
		BLT_ASSERT(false, "Cannot determine type of stream");
		return ValueType::Int;
	}

	ShaderType ShaderProgramFactory::GetCurrentShader() const
	{
		return m_CurrentShader;
	}

	ShaderValuePtr ShaderProgramFactory::GetStream(ShaderStream stream)
	{
		auto it = std::find_if(m_StreamValues.begin(), m_StreamValues.end(), [stream](const StreamValueInfo& value)
		{
			return stream == value.Stream;
		});
		if (it == m_StreamValues.end())
		{
			ShaderValuePtr value = std::make_shared<ShaderValue>(OutputValueType::Stream, GetValueTypeOfStream(stream), GetCurrentShader(), GetStreamName(stream));
			m_StreamValues.push_back({ stream, value });
			return value;
		}
		return it->Value;
	}

	ShaderValuePtr ShaderProgramFactory::RendererUniform(Bolt::RendererUniform uniform)
	{
		ShaderValuePtr value = std::make_shared<ShaderValue>(OutputValueType::RendererUniform, GetRendererUniformType(uniform), GetCurrentShader(), GetRendererUniformName(uniform));
		m_Uniforms.push_back(value);
		return value;
	}

	ShaderValuePtr ShaderProgramFactory::Uniform(ValueType type)
	{
		ShaderValuePtr value = std::make_shared<ShaderValue>(OutputValueType::UserUniform, type, GetCurrentShader(), GetNextUniformName());
		m_Uniforms.push_back(value);
		return value;
	}

	ShaderValuePtr ShaderProgramFactory::Pass(ShaderValuePtr value)
	{
		BLT_ASSERT(value->Shader() == GetCurrentShader(), "Shader mismatch");
		ShaderValuePtr pass = std::make_shared<ShaderValue>(OutputValueType::PassValue, value->Type(), GetCurrentShader(), GetNextPassName());
		m_PassValues.push_back({ pass, value });
		return pass;
	}

	ShaderValuePtr ShaderProgramFactory::FuncResult(const FunctionDecl& declaration, const std::vector<ShaderValuePtr>& inputs)
	{
		for (const auto& ptr : inputs)
		{
			BLT_ASSERT(ptr->Shader() == GetCurrentShader(), "Shader mismatch");
		}
		ShaderValuePtr value = std::make_shared<ShaderValue>(OutputValueType::Value, declaration.OutputType(), GetCurrentShader(), GetNextValueName());
		m_Values.push_back({ value, FunctionDef::GetAssignmentString(declaration, inputs) });
		return value;
	}

	ShaderValuePtr ShaderProgramFactory::FuncResult(const BuiltInFunc& declaration, const std::vector<ShaderValuePtr>& inputs)
	{
		for (const auto& ptr : inputs)
		{
			BLT_ASSERT(ptr->Shader() == GetCurrentShader(), "Shader mismatch");
		}
		BuiltInFunc::OverloadInstance instance = declaration.GetSource(inputs);
		ShaderValuePtr value = std::make_shared<ShaderValue>(OutputValueType::Value, instance.Output, GetCurrentShader(), GetNextValueName());
		m_Values.push_back({ value, instance.Source });
		return value;
	}

	void ShaderProgramFactory::SetAttribute(ShaderAttribute attribute, ShaderValuePtr value)
	{
		BLT_ASSERT(value->Shader() == GetCurrentShader(), "Shader mismatch");
		BLT_ASSERT(value->Type() == GetAttributeType(attribute), "Type mismatch");
		auto it = std::find_if(m_Attributes.begin(), m_Attributes.end(), [attribute](const ShaderAttributeInfo& value)
		{
			return attribute == value.Attribute;
		});
		if (it == m_Attributes.end())
		{
			m_Attributes.push_back({ attribute, value });
			return;
		}
		it->Value = value;
	}

	blt::string ShaderProgramFactory::CreateSource() const
	{
		blt::string source = "#version 430 core\n\n";

		source += m_Context.GetSource();
		if (m_Context.FunctionCount() > 0)
		{
			source += "\n\n";
		}

		for (const auto& stream : m_StreamValues)
		{
			source += "layout(location = " + std::to_string(IndexOfStream(stream.Stream)) + ") in " + FunctionDef::GetTypeString(stream.Value->Type()) + " " + stream.Value->VarName() + ";\n";
		}
		if (m_StreamValues.size() > 0)
		{
			source += '\n';
		}

		for (const auto& uniform : m_Uniforms)
		{
			source += "uniform " + FunctionDef::GetTypeString(uniform->Type()) + " " + uniform->VarName() + ";\n";
		}
		if (m_Uniforms.size() > 0)
		{
			source += '\n';
		}

		for (const auto& pass : m_PassedToMe)
		{
			source += "in " + FunctionDef::GetTypeString(pass->Type()) + " " + pass->VarName() + ";\n";
		}
		if (m_PassedToMe.size() > 0)
		{
			source += '\n';
		}

		for (const auto& pass : m_PassValues)
		{
			source += "out " + FunctionDef::GetTypeString(pass.Value->Type()) + " " + pass.Value->VarName() + ";\n";
		}
		if (m_CurrentShader == ShaderType::Fragment)
		{
			source += "layout(location = 0) out vec4 " + GetAttributeName(ShaderAttribute::FragColor) + ";\n";
			source += '\n';
		}
		else if (m_PassValues.size() > 0)
		{
			source += '\n';
		}

		source += "void main(void)\n{\n";

		for (const auto& value : m_Values)
		{
			source += '\t' + FunctionDef::GetTypeString(value.Value->Type()) + ' ' + value.Value->VarName() + " = " + value.AssignmentString + ";\n";
		}

		for (const auto& attrib : m_Attributes)
		{
			source += '\t' + GetAttributeName(attrib.Attribute) + " = " + attrib.Value->VarName() + ";\n";
		}

		for (const auto& pass : m_PassValues)
		{
			source += '\t' + pass.Value->VarName() + " = " + pass.PassedValue->VarName() + ";\n";
		}
		source += "}";
		return source;
	}

	blt::string ShaderProgramFactory::GetStreamName(ShaderStream stream)
	{
		switch (stream)
		{
		case ShaderStream::Position:
			return "in_Position";
		case ShaderStream::Normal:
			return "in_Normal";
		case ShaderStream::TexCoord:
			return "in_TexCoord";
		case ShaderStream::Color:
			return "in_Color";
		case ShaderStream::Tangent:
			return "in_Tangent";
		}
		BLT_ASSERT(false, "Cannot find name for stream");
		return "";
	}

	int ShaderProgramFactory::IndexOfStream(ShaderStream stream)
	{
		return (int)stream;
	}

	blt::string ShaderProgramFactory::GetAttributeName(ShaderAttribute attribute)
	{
		switch (attribute)
		{
		case ShaderAttribute::Position:
			return "gl_Position";
		case ShaderAttribute::FragColor:
			return "FragColor";
		}
		BLT_ASSERT(false, "Cannot find name for attribute");
		return "";
	}

	ValueType ShaderProgramFactory::GetAttributeType(ShaderAttribute attribute)
	{
		switch (attribute)
		{
		case ShaderAttribute::Position:
			return ValueType::Vector4f;
		case ShaderAttribute::FragColor:
			return ValueType::Vector4f;
		}
		BLT_ASSERT(false, "Cannot find name for attribute");
		return ValueType::Void;
	}

	blt::string ShaderProgramFactory::GetRendererUniformName(Bolt::RendererUniform uniform)
	{
		switch (uniform)
		{
		case Bolt::RendererUniform::ModelMatrix:
			return "u_ModelMatrix";
		case Bolt::RendererUniform::ViewMatrix:
			return "u_ViewMatrix";
		case Bolt::RendererUniform::ProjectionMatrix:
			return "u_ProjectionMatrix";
		case Bolt::RendererUniform::Time:
			return "u_Time";
		}
		BLT_ASSERT(false, "Cannot find name for uniform");
		return "";
	}

	ValueType ShaderProgramFactory::GetRendererUniformType(Bolt::RendererUniform uniform)
	{
		switch (uniform)
		{
		case Bolt::RendererUniform::ModelMatrix:
			return ValueType::Matrix4f;
		case Bolt::RendererUniform::ViewMatrix:
			return ValueType::Matrix4f;
		case Bolt::RendererUniform::ProjectionMatrix:
			return ValueType::Matrix4f;
		case Bolt::RendererUniform::Time:
			return ValueType::Float;
		}
		BLT_ASSERT(false, "Cannot find type of uniform");
		return ValueType::Void;
	}

	void ShaderProgramFactory::AddValuePassedToMe(ShaderValuePtr value)
	{
		value->SetShader(m_CurrentShader);
		m_PassedToMe.push_back(value);
	}

	blt::string ShaderProgramFactory::GetShaderName() const
	{
		switch (m_CurrentShader)
		{
		case ShaderType::Vertex:
			return "Vertex";
		case ShaderType::Geometry:
			return "Geometry";
		case ShaderType::Fragment:
			return "Fragment";
		}
		BLT_ASSERT(false, "Cannot find name for shader");
		return "";
	}

	blt::string ShaderProgramFactory::GetNextUniformName() const
	{
		return "Uniform_" + GetShaderName() + std::to_string(m_Uniforms.size());
	}

	blt::string ShaderProgramFactory::GetNextPassName() const
	{
		return "Pass_" + GetShaderName() + std::to_string(m_PassValues.size());
	}

	blt::string ShaderProgramFactory::GetNextValueName() const
	{
		return "Value_" + GetShaderName() + std::to_string(m_Values.size());
	}

}