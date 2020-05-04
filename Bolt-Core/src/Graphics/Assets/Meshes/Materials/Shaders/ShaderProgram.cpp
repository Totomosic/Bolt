#include "bltpch.h"
#include "ShaderProgram.h"

#include "Components/Operations/SetValueOp.h"
#include "Components/Operations/MathOps/AddAssignOp.h"
#include "Components/Operations/MathOps/SubAssignOp.h"
#include "Components/Operations/MathOps/MulAssignOp.h"
#include "Components/Operations/MathOps/DivAssignOp.h"

namespace Bolt
{

	ShaderProgram::ShaderProgram(ShaderStage type)
		: m_Builder(type), m_ShaderStage(type), m_UserUniforms(), m_RendererUniforms()
	{
	
	}

	ShaderStage ShaderProgram::Type() const
	{
		return m_ShaderStage;
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

	ShaderVariablePtr ShaderProgram::Uniform(const std::string& linkName, ValueType type, std::shared_ptr<UniformValueContainer> defaultValue)
	{
		ShaderVariablePtr var = GetGlobalScope().DeclareVar(type, "uniform");
		std::string name = linkName;
		if (name == NAMELESS_UNIFORM)
		{
			name = "uniform_" + std::to_string(m_UserUniforms.size());
		}
		m_UserUniforms.push_back({ name, var.get(), 0, std::move(defaultValue) });
		return var;
	}

	ShaderVariablePtr ShaderProgram::UniformArray(const std::string& linkName, ValueType type, uint32_t length)
	{
		ShaderVariablePtr var = GetGlobalScope().DeclareArray(type, ShaderLiteral::FromInt((int)length), "uniform");
		m_UserUniforms.push_back({ linkName, var.get(), (int)length });
		return var;
	}

	ShaderVariablePtr ShaderProgram::RendererUniform(Bolt::RendererUniform uniform)
	{
		if (GetTypeDimOfRendererUniform(uniform) == ValueTypeDim::Single)
		{
			ShaderVariablePtr var = GetGlobalScope().DeclareVar(GetTypeOfRendererUniform(uniform), "uniform");
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
		return GetMainScope().DeclareVar(type, "");
	}

	ShaderVariablePtr ShaderProgram::DefineVar(const ShaderValuePtr& value)
	{
		return GetMainScope().DefineVar(value, "");
	}

	ShaderPassVariablePtr ShaderProgram::DeclarePassOut(ValueType type, PassType passType)
	{
		return GetGlobalScope().DeclarePassOut(type, passType, GetGLSLStringFromPassType(passType));
	}

	ShaderPassVariablePtr ShaderProgram::DeclarePassIn(const ShaderPassVariablePtr& passOut)
	{
		return GetGlobalScope().DeclarePassIn(passOut, GetGLSLStringFromPassType(passOut->GetPassType()));
	}

	ShaderVariablePtr ShaderProgram::DeclareArray(ValueType type, uint32_t length)
	{
		return GetMainScope().DeclareArray(type, ShaderLiteral::FromInt((int)length));
	}

	void ShaderProgram::SetVariable(const ShaderLValuePtr& var, const ShaderValuePtr& value)
	{
		GetMainScope().SetVariable(var, value);
	}

	void ShaderProgram::AddAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value)
	{
		GetMainScope().AddAssign(var, value);
	}

	void ShaderProgram::SubAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value)
	{
		GetMainScope().SubAssign(var, value);
	}

	void ShaderProgram::MulAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value)
	{
		GetMainScope().MulAssign(var, value);
	}

	void ShaderProgram::DivAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value)
	{
		GetMainScope().DivAssign(var, value);
	}

	ForLoopScope& ShaderProgram::For(const ShaderVariablePtr& counter, const ShaderValuePtr& initial, const ShaderValuePtr& condition, std::unique_ptr<ShaderOp>&& iteration)
	{
		return AddScope<ForLoopScope>(std::make_unique<SetValueOp>(counter, initial), condition, std::move(iteration));
	}

	IfScope& ShaderProgram::If(const ShaderValuePtr& condition)
	{
		return AddScope<IfScope>(condition);
	}

	bool ShaderProgram::HasFunction(const std::string& name) const
	{
		return GetGlobalScope().HasFunction(name);
	}

	FunctionScope& ShaderProgram::GetFunction(const std::string& name) const
	{
		return GetGlobalScope().GetFunction(name);
	}

	FunctionScope& ShaderProgram::DefineFunction(const std::string& name, const ValueTypeInfo& returnType, const std::vector<ValueTypeInfo>& inputs)
	{
		return GetGlobalScope().DefineFunction(name, returnType, inputs);
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

	ShaderVariablePtr ShaderProgram::PrivateStream(int streamIndex)
	{
		BLT_ASSERT(false, "Can only use shader streams in vertex shader");
		return ShaderVariablePtr();
	}

}