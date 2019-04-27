#pragma once
#include "Components/__Components__.h"

namespace Bolt
{

	struct BLT_API UserUniformInfo
	{
	public:
		blt::string LinkName;
		blt::string VarName;
		ValueType Type;
	};

	struct BLT_API UserUniformPtr
	{
	public:
		blt::string LinkName;
		const ShaderVariable* Var;
	};

	struct BLT_API RendererUniformInfo
	{
	public:
		blt::string VarName;
		RendererUniform Uniform;
	};

	struct BLT_API RendererUniformPtr
	{
	public:
		RendererUniform Uniform;
		const ShaderVariable* Var;
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
		ShaderScope* m_CurrentScope;

		std::vector<UserUniformPtr> m_UserUniforms;
		std::vector<RendererUniformPtr> m_RendererUniforms;

	protected:
		ShaderProgram(ShaderType type);

	public:
		ShaderType Type() const;
		ShaderScope& GetCurrentScope() const;

		ShaderVariablePtr Stream(ShaderStream stream);
		ShaderVariablePtr Uniform(const blt::string& linkName, ValueType type);
		ShaderVariablePtr RendererUniform(Bolt::RendererUniform uniform);
		ShaderVariablePtr DeclareVar(ValueType type);
		ShaderVariablePtr DefineVar(const ShaderValuePtr& value);
		ShaderVariablePtr DeclarePassOut(ValueType type);
		ShaderVariablePtr DeclarePassIn(const ShaderVariablePtr& passOut);

		template<typename T>
		ShaderVariablePtr Uniform(const blt::string& linkName)
		{
			return Uniform(linkName, GetValueType<T>());
		}

		void AddMainScope();
		void SetCurrentScope(ShaderScope* scope);
		virtual CompiledShaderProgram Compile() const = 0;
		virtual void Reset();

		template<typename T, typename... Args>
		T* AddScope(Args&& ... args)
		{
			std::unique_ptr<T> scope = std::make_unique<T>(std::forward<Args>(args)...);
			T* ptr = scope.get();
			GetCurrentScope().AddChildScope(std::move(scope));
			SetCurrentScope((ShaderScope*)ptr);
			return ptr;
		}

		template<typename T, typename... Args>
		void AddOperation(Args&& ... args)
		{
			std::unique_ptr<T> op = std::make_unique<T>(std::forward<Args>(args)...);
			GetCurrentScope().AddOperation(std::move(op));
		}

	protected:
		void CompileUniformVariables(CompiledShaderProgram& program) const;

	};

}