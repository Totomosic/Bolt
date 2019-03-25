#include "Types.h"
#include "FragmentShader.h"

namespace Bolt
{

	FragmentShader::FragmentShader() : ShaderProgram(ShaderType::Fragment),
		m_FragColor(nullptr)
	{
		
	}

	void FragmentShader::SetFragColor(ShaderValuePtr value)
	{
		m_FragColor = std::make_shared<FragColorAttribute>(std::move(value));
	}

	CompiledShaderProgram FragmentShader::Compile() const
	{
		BLT_ASSERT(m_FragColor != nullptr, "FragColor attribute has not been set");
		CompiledShaderProgram result;
		ShaderBuilder builder(m_ShaderType);
		m_FragColor->Build(builder);

		for (const ShaderPassValuePtr& pass : m_PassValues)
		{
			pass->Build(builder);
		}

		result.Source = builder.GetSource();
		return result;
	}

	void FragmentShader::Reset()
	{
		ShaderProgram::Reset();
		m_FragColor = nullptr;
	}

}