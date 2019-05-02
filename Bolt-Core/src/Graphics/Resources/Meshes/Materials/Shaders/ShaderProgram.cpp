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
		BLT_ASSERT(&GetCurrentScope() == &m_Builder.GetGlobalScope(), "Stream should be declared in global scope, before main() scope");
		return GetCurrentScope().DeclareVariable(GetTypeOfShaderStream(stream), "layout(location = " + std::to_string((int)stream) + ") in");
	}

	ShaderVariablePtr ShaderProgram::Uniform(const blt::string& linkName, ValueType type, std::shared_ptr<UniformValueContainer> defaultValue)
	{
		BLT_ASSERT(&GetCurrentScope() == &m_Builder.GetGlobalScope(), "Uniforms should be declared in global scope, before main() scope");
		ShaderVariablePtr var = GetCurrentScope().DeclareVariable(type, "uniform");
		m_UserUniforms.push_back({ linkName, var.get(), 0, std::move(defaultValue) });
		return var;
	}

	ShaderVariablePtr ShaderProgram::UniformArray(const blt::string& linkName, ValueType type, size_t length)
	{
		BLT_ASSERT(&GetCurrentScope() == &m_Builder.GetGlobalScope(), "Uniforms should be declared in global scope, before main() scope");
		ShaderVariablePtr var = GetCurrentScope().DeclareArray(type, ShaderLiteral::FromInt((int)length), "uniform");
		m_UserUniforms.push_back({ linkName, var.get(), (int)length });
		return var;
	}

	ShaderVariablePtr ShaderProgram::RendererUniform(Bolt::RendererUniform uniform)
	{
		BLT_ASSERT(&GetCurrentScope() == &m_Builder.GetGlobalScope(), "Uniforms should be declared in global scope, before main() scope");
		if (GetTypeDimOfRendererUniform(uniform) == ValueTypeDim::Single)
		{
			ShaderVariablePtr var = GetCurrentScope().DeclareVariable(GetTypeOfRendererUniform(uniform), "uniform");
			m_RendererUniforms.push_back({ uniform, var.get(), 0 });
			return var;
		}
		else
		{
			int length = GetArrayLengthOfRendererUniform(uniform);
			ShaderVariablePtr var = GetCurrentScope().DeclareArray(GetTypeOfRendererUniform(uniform), ShaderLiteral::FromInt(length), "uniform");
			m_RendererUniforms.push_back({ uniform, var.get(), length });
			return var;
		}
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
		BLT_ASSERT(&GetCurrentScope() == &m_Builder.GetGlobalScope(), "Pass values should be declared in global scope, before main() scope");
		return GetCurrentScope().DeclarePassOut(type);
	}

	ShaderVariablePtr ShaderProgram::DeclarePassIn(const ShaderVariablePtr& passOut)
	{
		BLT_ASSERT(&GetCurrentScope() == &m_Builder.GetGlobalScope(), "Pass values should be declared in global scope, before main() scope");
		return GetCurrentScope().DeclarePassIn(passOut);
	}

	ShaderVariablePtr ShaderProgram::DeclareArray(ValueType type, size_t length)
	{
		return GetCurrentScope().DeclareArray(type, ShaderLiteral::FromInt((int)length));
	}

	void ShaderProgram::SetVariable(const ShaderLValuePtr& var, const ShaderValuePtr& value)
	{
		AddOperation<SetValueOp>(var, value);
	}

	void ShaderProgram::AddAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value)
	{
		AddOperation<AddAssignOp>(var, value);
	}

	void ShaderProgram::SubAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value)
	{
		AddOperation<SubAssignOp>(var, value);
	}

	void ShaderProgram::MulAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value)
	{
		AddOperation<MulAssignOp>(var, value);
	}

	void ShaderProgram::DivAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value)
	{
		AddOperation<DivAssignOp>(var, value);
	}

	MainScope* ShaderProgram::AddMainScope()
	{
		return AddScope<MainScope>();
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
			program.UserUniforms.push_back({ user.LinkName, user.Var->GetVarName(), user.Var->Type(), user.Var->TypeDimension(), user.Length, user.DefaultValue });
		}
		for (const auto& renderer : m_RendererUniforms)
		{
			program.RendererUniforms.push_back({ renderer.Var->GetVarName(), renderer.Uniform, renderer.Length });
		}
	}

}