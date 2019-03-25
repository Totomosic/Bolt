#include "Types.h"
#include "ShaderProgram.h"

namespace Bolt
{

	ShaderProgram::ShaderProgram(ShaderType type)
		: m_ShaderType(type), m_PassValues(), m_RendererUniforms(), m_Uniforms()
	{
	
	}

	ShaderType ShaderProgram::Type() const
	{
		return m_ShaderType;
	}

	ShaderPassValuePtr ShaderProgram::Pass(ShaderValuePtr value)
	{
		ShaderPassValuePtr pass = std::make_shared<ShaderPassValue>(std::move(value));
		m_PassValues.push_back(pass);
		return std::move(pass);
	}

	ShaderRendererUniformPtr ShaderProgram::RendererUniform(Bolt::RendererUniform uniform)
	{
		ShaderRendererUniformPtr u = std::make_shared<ShaderRendererUniform>(uniform);
		m_RendererUniforms.push_back(u);
		return u;
	}

	ShaderUniformPtr ShaderProgram::Uniform(ValueType type)
	{
		ShaderUniformPtr u = std::make_shared<ShaderUniform>(type);
		m_Uniforms.push_back(u);
		return u;
	}

	ShaderUniformPtr ShaderProgram::Uniform(ShaderLiteralPtr defaultValue)
	{
		ShaderUniformPtr u = std::make_shared<ShaderUniform>(std::move(defaultValue));
		m_Uniforms.push_back(u);
		return u;
	}

	void ShaderProgram::Reset()
	{
		m_PassValues.clear();
		m_RendererUniforms.clear();
		m_Uniforms.clear();
	}

}