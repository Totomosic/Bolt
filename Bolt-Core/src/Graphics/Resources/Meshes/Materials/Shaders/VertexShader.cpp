#include "Types.h"
#include "VertexShader.h"
#include "Components/ShaderBuilder.h"

namespace Bolt
{

	VertexShader::VertexShader() : ShaderProgram(ShaderType::Vertex),
		m_VertexPosition(nullptr), m_PositionStream(ShaderStream::Position()), m_NormalStream(ShaderStream::Normal()), m_TexCoordStream(ShaderStream::TexCoord()), m_ColorStream(ShaderStream::Color()), m_TangentStream(ShaderStream::Tangent())
	{
	
	}

	const ShaderStreamPtr& VertexShader::Position() const
	{
		return m_PositionStream;
	}

	const ShaderStreamPtr& VertexShader::Normal() const
	{
		return m_NormalStream;
	}

	const ShaderStreamPtr& VertexShader::TexCoord() const
	{
		return m_TexCoordStream;
	}

	const ShaderStreamPtr& VertexShader::Color() const
	{
		return m_ColorStream;
	}

	const ShaderStreamPtr& VertexShader::Tangent() const
	{
		return m_TangentStream;
	}

	void VertexShader::SetVertexPosition(ShaderValuePtr value)
	{
		m_VertexPosition = std::make_shared<ShaderAttribute>(ShaderAttribute::VERTEX_POSITION, std::move(value));
	}

	CompiledShaderProgram VertexShader::Compile() const
	{
		BLT_ASSERT(m_VertexPosition != nullptr, "VertexPosition attribute has not been set");
		CompiledShaderProgram result;
		ShaderBuilder builder(m_ShaderType);
		m_VertexPosition->Build(builder);

		for (const ShaderPassValuePtr& pass : m_PassValues)
		{
			pass->Build(builder);
			pass->m_ShaderType = ShaderType::Ignore;
		}

		result.Source = builder.GetSource();
		CompileUniformVariables(result);
		return result;
	}

	void VertexShader::Reset()
	{
		ShaderProgram::Reset();
		m_VertexPosition = nullptr;
	}

}