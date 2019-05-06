#pragma once
#include "Components/Values/ShaderVariable.h"
#include "Components/GlobalScope.h"
#include "Components/MainScope.h"
#include "Components/ShaderBuilder.h"
#include "UniformValue.h"

namespace Bolt
{

	struct BLT_API UserUniformInfo
	{
	public:
		blt::string LinkName;
		blt::string VarName;
		ValueType Type;
		ValueTypeDim Dimension;
		int Length;
		std::shared_ptr<UniformValueContainer> DefaultValue;
	};

	struct BLT_API UserUniformPtr
	{
	public:
		blt::string LinkName;
		const ShaderVariable* Var;
		int Length;
		std::shared_ptr<UniformValueContainer> DefaultValue = nullptr;
	};

	struct BLT_API RendererUniformInfo
	{
	public:
		blt::string VarName;
		RendererUniform Uniform;
		int Length;
	};

	struct BLT_API RendererUniformPtr
	{
	public:
		RendererUniform Uniform;
		const ShaderVariable* Var;
		int Length;
	};

	struct BLT_API CompiledShaderProgram
	{
	public:
		blt::string Source;
		std::vector<UserUniformInfo> UserUniforms;
		std::vector<RendererUniformInfo> RendererUniforms;
	};

	class BLT_API ShaderProgram
	{
	protected:
		mutable ShaderBuilder m_Builder;
		ShaderType m_ShaderType;

		std::vector<UserUniformPtr> m_UserUniforms;
		std::vector<RendererUniformPtr> m_RendererUniforms;

	protected:
		ShaderProgram(ShaderType type);

	public:
		ShaderType Type() const;

		const GlobalScope& GetGlobalScope() const;
		GlobalScope& GetGlobalScope();
		const MainScope& GetMainScope() const;
		MainScope& GetMainScope();

		ShaderVariablePtr Stream(ShaderStream stream);
		ShaderVariablePtr Uniform(const blt::string& linkName, ValueType type, std::shared_ptr<UniformValueContainer> defaultValue = nullptr);
		ShaderVariablePtr UniformArray(const blt::string& linkName, ValueType type, size_t length);
		ShaderVariablePtr RendererUniform(Bolt::RendererUniform uniform);
		ShaderVariablePtr DeclareVar(ValueType type);
		ShaderVariablePtr DefineVar(const ShaderValuePtr& value);
		ShaderVariablePtr DeclarePassOut(ValueType type);
		ShaderVariablePtr DeclarePassIn(const ShaderVariablePtr& passOut);
		ShaderVariablePtr DeclareArray(ValueType type, size_t length);

		template<typename T>
		ShaderVariablePtr DeclareVar()
		{
			return DeclareVar(GetValueType<T>());
		}

		template<typename T>
		ShaderVariablePtr Uniform(const blt::string& linkName)
		{
			return Uniform(linkName, GetValueType<T>());
		}

		template<typename T>
		ShaderVariablePtr Uniform(const blt::string& linkName, const T& defaultValue)
		{
			return Uniform(linkName, GetValueType<T>(), std::make_shared<UniformValue<T>>(defaultValue));
		}

		template<typename T>
		ShaderVariablePtr UniformArray(const blt::string& linkName, size_t length)
		{
			return UniformArray(linkName, GetValueType<T>(), length);
		}

		template<typename T>
		ShaderVariablePtr DeclarePassOut()
		{
			return DeclarePassOut(GetValueType<T>());
		}

		template<typename T>
		ShaderVariablePtr DeclareArray(size_t length)
		{
			return DeclareArray(GetValueType<T>(), length);
		}

		void SetVariable(const ShaderLValuePtr& var, const ShaderValuePtr& value);
		void AddAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value);
		void SubAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value);
		void MulAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value);
		void DivAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value);

		virtual CompiledShaderProgram Compile() const = 0;
		virtual void Reset();

		template<typename T, typename... Args>
		T& AddScope(Args&& ... args)
		{
			return GetMainScope().AddScope<T>(GetMainScope().GetScopeIndex() + 1, std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		void AddOperation(Args&& ... args)
		{
			std::unique_ptr<T> op = std::make_unique<T>(std::forward<Args>(args)...);
			GetMainScope().AddOperation(std::move(op));
		}

	protected:
		void CompileUniformVariables(CompiledShaderProgram& program) const;

	};

}