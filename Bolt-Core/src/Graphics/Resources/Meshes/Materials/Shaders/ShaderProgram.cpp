#include "Types.h"
#include "ShaderProgram.h"

namespace Bolt
{

	ShaderProgram::ShaderProgram(ShaderType type)
		: m_Builder(type), m_ShaderType(type), m_UserUniforms(), m_RendererUniforms()
	{
	
	}

	ShaderType ShaderProgram::Type() const
	{
		return m_ShaderType;
	}

	const GlobalScope& ShaderProgram::GetGlobalScope() const
	{
		return m_Builder.GetGlobalScope();
	}

	GlobalScope& ShaderProgram::GetGlobalScope()
	{
		return m_Builder.GetGlobalScope();
	}

	const MainScope& ShaderProgram::GetMainScope() const
	{
		return m_Builder.GetMainScope();
	}

	MainScope& ShaderProgram::GetMainScope()
	{
		return m_Builder.GetMainScope();
	}

	ShaderVariablePtr ShaderProgram::Stream(ShaderStream stream)
	{
		return GetGlobalScope().DeclareVariable(GetTypeOfShaderStream(stream), "layout(location = " + std::to_string((int)stream) + ") in");
	}

	ShaderVariablePtr ShaderProgram::Uniform(const blt::string& linkName, ValueType type, std::shared_ptr<UniformValueContainer> defaultValue)
	{
		ShaderVariablePtr var = GetGlobalScope().DeclareVariable(type, "uniform");
		m_UserUniforms.push_back({ linkName, var.get(), 0, std::move(defaultValue) });
		return var;
	}

	ShaderVariablePtr ShaderProgram::UniformArray(const blt::string& linkName, ValueType type, size_t length)
	{
		ShaderVariablePtr var = GetGlobalScope().DeclareArray(type, ShaderLiteral::FromInt((int)length), "uniform");
		m_UserUniforms.push_back({ linkName, var.get(), (int)length });
		return var;
	}

	ShaderVariablePtr ShaderProgram::RendererUniform(Bolt::RendererUniform uniform)
	{
		if (GetTypeDimOfRendererUniform(uniform) == ValueTypeDim::Single)
		{
			ShaderVariablePtr var = GetGlobalScope().DeclareVariable(GetTypeOfRendererUniform(uniform), "uniform");
			m_RendererUniforms.push_back({ uniform, var.get(), 0 });
			return var;
		}
		else
		{
			int length = GetArrayLengthOfRendererUniform(uniform);
			ShaderVariablePtr var = GetGlobalScope().DeclareArray(GetTypeOfRendererUniform(uniform), ShaderLiteral::FromInt(length), "uniform");
			m_RendererUniforms.push_back({ uniform, var.get(), length });
			return var;
		}
	}

	ShaderVariablePtr ShaderProgram::DeclareVar(ValueType type)
	{
		return GetMainScope().DeclareVariable(type, "");
	}

	ShaderVariablePtr ShaderProgram::DefineVar(const ShaderValuePtr& value)
	{
		return GetMainScope().DefineVariable(value, "");
	}

	ShaderVariablePtr ShaderProgram::DeclarePassOut(ValueType type)
	{
		return GetGlobalScope().DeclarePassOut(type);
	}

	ShaderVariablePtr ShaderProgram::DeclarePassIn(const ShaderVariablePtr& passOut)
	{
		return GetGlobalScope().DeclarePassIn(passOut);
	}

	ShaderVariablePtr ShaderProgram::DeclareArray(ValueType type, size_t length)
	{
		return GetMainScope().DeclareArray(type, ShaderLiteral::FromInt((int)length));
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