#include "bltpch.h"
#include "VertexShader.h"
#include "Components/ShaderBuilder.h"

namespace Bolt
{

	VertexShader::VertexShader() : ShaderProgram(ShaderType::Vertex),
		m_PositionStream(nullptr), m_NormalStream(nullptr), m_TexCoordStream(nullptr), m_ColorStream(nullptr), m_TangentStream(nullptr), m_VertexPosition(nullptr)
	{
		m_VertexPosition = std::make_shared<ShaderVariable>(ValueType::Vector4f);
		m_VertexPosition->SetVarName("gl_Position");
		m_PositionStream = Stream(ShaderStream::Position);
		m_NormalStream = Stream(ShaderStream::Normal);
		m_TexCoordStream = Stream(ShaderStream::TexCoord);
		m_ColorStream = Stream(ShaderStream::Color);
		m_TangentStream = Stream(ShaderStream::Tangent);
	}

	const ShaderVariablePtr& VertexShader::Position() const
	{
		return m_PositionStream;
	}

	const ShaderVariablePtr& VertexShader::Normal() const
	{
		return m_NormalStream;
	}

	const ShaderVariablePtr& VertexShader::TexCoord() const
	{
		return m_TexCoordStream;
	}

	const ShaderVariablePtr& VertexShader::Color() const
	{
		return m_ColorStream;
	}

	const ShaderVariablePtr& VertexShader::Tangent() const
	{
		return m_TangentStream;
	}

	const ShaderVariablePtr& VertexShader::VertexPosition() const
	{
		return m_VertexPosition;
	}

	void VertexShader::SetVertexPosition(const ShaderValuePtr& value)
	{
		SetVariable(m_VertexPosition, value);
	}

	CompiledShaderProgram VertexShader::Compile() const
	{
		CompiledShaderProgram result;
		result.Source = m_Builder.Build();
		CompileUniformVariables(result);
		return result;
	}

	void VertexShader::Reset()
	{
		ShaderProgram::Reset();
	}

}