#include "Types.h"
#include "ShaderProgram.h"

namespace Bolt
{

	ShaderProgram::ShaderProgram(ShaderType type)
		: m_Builder(type), m_ShaderType(type), m_PassValues(), m_RendererUniforms(), m_Uniforms()
	{
	
	}

	ShaderType ShaderProgram::Type() const
	{
		return m_ShaderType;
	}

	ShaderVariablePtr ShaderProgram::Pass(ShaderValuePtr value)
	{
		ShaderVariablePtr pass = std::make_shared<ShaderVariable>(std::move(value), ShaderType::Fragment);
		m_PassValues.push_back(pass);
		return pass;
	}

	ShaderVariablePtr ShaderProgram::RendererUniform(Bolt::RendererUniform uniform)
	{
		ShaderVariablePtr u = std::make_shared<ShaderVariable>(uniform);
		m_RendererUniforms.push_back(u);
		return u;
	}

	ShaderVariablePtr ShaderProgram::Uniform(const blt::string& linkName, ValueType type)
	{
		ShaderVariablePtr u = std::make_shared<ShaderVariable>(type);
		m_Uniforms.push_back({ linkName, u });
		return u;
	}

	ShaderVariablePtr ShaderProgram::Uniform(const blt::string& linkName, ShaderLiteralPtr defaultValue)
	{
		ShaderVariablePtr u = std::make_shared<ShaderVariable>(std::move(defaultValue));
		m_Uniforms.push_back({ linkName, u });
		return u;
	}

	void ShaderProgram::Reset()
	{
		m_PassValues.clear();
		m_RendererUniforms.clear();
		m_Uniforms.clear();
	}

	void ShaderProgram::CompileUniformVariables(CompiledShaderProgram& program) const
	{
		for (const UserUniformMapping& uniform : m_Uniforms)
		{
			program.UserUniforms.push_back({ uniform.LinkName, uniform.Uniform->GetVarName(), uniform.Uniform->Type() });
		}
		for (const ShaderVariablePtr& uniform : m_RendererUniforms)
		{
			program.RendererUniforms.push_back({ uniform->GetVarName(), uniform->UniformType() });
		}
	}

}