#include "Types.h"
#include "ShaderProgram.h"

namespace Bolt
{

	ShaderProgram::ShaderProgram(ShaderType type)
		: m_Builder(type), m_ShaderType(type), m_CurrentScope(&m_Builder.GetGlobalScope()), m_UserUniforms(), m_RendererUniforms()
	{
	
	}

	ShaderType ShaderProgram::Type() const
	{
		return m_ShaderType;
	}

	ShaderScope& ShaderProgram::GetCurrentScope() const
	{
		return *m_CurrentScope;
	}

	ShaderVariablePtr ShaderProgram::Stream(ShaderStream stream)
	{
		return GetCurrentScope().DeclareVariable(GetTypeOfShaderStream(stream), "layout(location = " + std::to_string((int)stream) + ") in");
	}

	ShaderVariablePtr ShaderProgram::Uniform(const blt::string& linkName, ValueType type)
	{
		ShaderVariablePtr var = GetCurrentScope().DeclareVariable(type, "uniform");
		m_UserUniforms.push_back({ linkName, var.get() });
		return var;
	}

	ShaderVariablePtr ShaderProgram::RendererUniform(Bolt::RendererUniform uniform)
	{
		ShaderVariablePtr var = GetCurrentScope().DeclareVariable(GetTypeOfRendererUniform(uniform), "uniform");
		m_RendererUniforms.push_back({ uniform, var.get() });
		return var;
	}

	ShaderVariablePtr ShaderProgram::DeclareVar(ValueType type)
	{
		return GetCurrentScope().DeclareVariable(type, "");
	}

	ShaderVariablePtr ShaderProgram::DefineVar(const ShaderValuePtr& value)
	{
		return GetCurrentScope().DefineVariable(value, "");
	}

	ShaderVariablePtr ShaderProgram::DeclarePassOut(ValueType type)
	{
		return GetCurrentScope().DeclarePassOut(type);
	}

	ShaderVariablePtr ShaderProgram::DeclarePassIn(const ShaderVariablePtr& passOut)
	{
		return GetCurrentScope().DeclarePassIn(passOut);
	}

	void ShaderProgram::AddMainScope()
	{
		AddScope<MainScope>(m_Builder.GetGlobalScope().GetScopeIndex() + 1, &m_Builder.GetGlobalScope());
	}

	void ShaderProgram::SetCurrentScope(ShaderScope* scope)
	{
		m_CurrentScope = scope;
	}

	void ShaderProgram::Reset()
	{
		m_UserUniforms.clear();
		m_RendererUniforms.clear();
	}

	void ShaderProgram::CompileUniformVariables(CompiledShaderProgram& program) const
	{
		for (const auto& user : m_UserUniforms)
		{
			program.UserUniforms.push_back({ user.LinkName, user.Var->GetVarName(), user.Var->Type() });
		}
		for (const auto& renderer : m_RendererUniforms)
		{
			program.RendererUniforms.push_back({ renderer.Var->GetVarName(), renderer.Uniform });
		}
	}

}