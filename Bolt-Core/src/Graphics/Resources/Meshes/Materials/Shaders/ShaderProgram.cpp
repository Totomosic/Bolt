#include "Types.h"
#include "ShaderProgram.h"

namespace Bolt
{

	ShaderProgram::ShaderProgram(ShaderType type)
		: m_Type(type), m_RendererUniforms(), m_UserUniforms(), m_PassValues()
	{
	
	}

	ShaderType ShaderProgram::Type() const
	{
		return m_Type;
	}

	ShaderRendererUniformPtr ShaderProgram::RendererUniform(Bolt::RendererUniform uniform)
	{
		auto it = std::find_if(m_RendererUniforms.begin(), m_RendererUniforms.end(), [uniform](const ShaderRendererUniformPtr& value)
		{
			return uniform == value->UniformType();
		});
		if (it != m_RendererUniforms.end())
		{
			return *it;
		}
		ShaderRendererUniformPtr ru = std::make_shared<ShaderRendererUniform>(uniform);
		m_RendererUniforms.push_back(ru);
		return ru;
	}

	ShaderUniformPtr ShaderProgram::Uniform(ValueType type)
	{
		ShaderUniformPtr ptr = std::make_shared<ShaderUniform>(type);
		m_UserUniforms.push_back(ptr);
		return ptr;
	}

	ShaderUniformPtr ShaderProgram::Uniform(ShaderValuePtr defaultValue)
	{
		ShaderUniformPtr ptr = std::make_shared<ShaderUniform>(std::move(defaultValue));
		m_UserUniforms.push_back(ptr);
		return ptr;
	}

	ShaderPassValuePtr ShaderProgram::Pass(ShaderValuePtr value)
	{
		ShaderPassValuePtr ptr = std::make_shared<ShaderPassValue>(std::move(value));
		m_PassValues.push_back(ptr);
		return ptr;
	}

	void ShaderProgram::Reset()
	{
		
	}

}