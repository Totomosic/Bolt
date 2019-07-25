#include "bltpch.h"
#include "VertexShader.h"
#include "Components/ShaderBuilder.h"

namespace Bolt
{

	VertexShader::VertexShader() : ShaderProgram(ShaderStage::Vertex),
		m_PositionStream(nullptr), m_NormalStream(nullptr), m_TexCoordStream(nullptr), m_ColorStream(nullptr), m_TangentStream(nullptr), m_VertexPosition(nullptr)
	{
		m_VertexPosition = std::make_shared<ShaderVariable>(ValueType::Vector4f);
		m_VertexPosition->SetVarName("gl_Position");
		m_PositionStream = PrivateStream(ShaderStream::Position);
		m_NormalStream = PrivateStream(ShaderStream::Normal);
		m_TexCoordStream = PrivateStream(ShaderStream::TexCoord);
		m_ColorStream = PrivateStream(ShaderStream::Color);
		m_TangentStream = PrivateStream(ShaderStream::Tangent);
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

	const ShaderVariablePtr& VertexShader::Stream(ShaderStream stream) const
	{
		switch (stream)
		{
		case ShaderStream::Position:
			return Position();
		case ShaderStream::Normal:
			return Normal();
		case ShaderStream::TexCoord:
			return TexCoord();
		case ShaderStream::Color:
			return Color();
		case ShaderStream::Tangent:
			return Tangent();
		}
		BLT_ASSERT(false, "Invalid shader stream");
		return Position();
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