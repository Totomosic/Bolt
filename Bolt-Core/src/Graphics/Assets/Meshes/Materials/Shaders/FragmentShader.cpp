#include "bltpch.h"
#include "FragmentShader.h"
#include "Components/Operations/DiscardOp.h"

namespace Bolt
{

	FragmentShader::FragmentShader() : ShaderProgram(ShaderStage::Fragment),
		m_FragColor(nullptr)
	{
		m_FragColor = GetGlobalScope().DeclareVar(ValueType::Vector4f, "layout(location=0) out");
	}

	const ShaderVariablePtr& FragmentShader::FragColor() const
	{
		return m_FragColor;
	}

	void FragmentShader::Discard()
	{
		AddOperation<DiscardOp>();
	}

	void FragmentShader::SetFragColor(const ShaderValuePtr& value)
	{
		SetVariable(m_FragColor, value);
	}

	CompiledShaderProgram FragmentShader::Compile() const
	{
		CompiledShaderProgram result;
		result.Source = m_Builder.Build();
		CompileUniformVariables(result);
		m_Builder.Reset();
		return result;
	}

	void FragmentShader::Reset()
	{
		ShaderProgram::Reset();
	}

}