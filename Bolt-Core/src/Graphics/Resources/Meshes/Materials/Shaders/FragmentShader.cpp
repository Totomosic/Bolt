#include "Types.h"
#include "FragmentShader.h"

namespace Bolt
{

	FragmentShader::FragmentShader() : ShaderProgram(ShaderType::Fragment),
		m_FragColor(nullptr)
	{
		m_FragColor = GetCurrentScope().DeclareVariable(ValueType::Vector4f, "layout(location = 0) out");
	}

	void FragmentShader::SetFragColor(const ShaderValuePtr& value)
	{
		AddOperation<SetValueOp>(m_FragColor, value);
	}

	CompiledShaderProgram FragmentShader::Compile() const
	{
		CompiledShaderProgram result;
		result.Source = m_Builder.Build();
		CompileUniformVariables(result);
		return result;
	}

	void FragmentShader::Reset()
	{
		ShaderProgram::Reset();
	}

}