#pragma once
#include "Components/Values/ShaderVariable.h"
#include "Components/GlobalScope.h"
#include "Components/MainScope.h"
#include "Components/ForLoopScope.h"
#include "Components/IfScope.h"
#include "Components/ShaderBuilder.h"
#include "UniformValue.h"

namespace Bolt
{

	struct BLT_API UserUniformInfo
	{
	public:
		std::string LinkName;
		std::string VarName;
		ValueType Type;
		ValueTypeDim Dimension;
		int Length;
		std::shared_ptr<UniformValueContainer> DefaultValue;
	};

	struct BLT_API UserUniformPtr
	{
	public:
		std::string LinkName;
		const ShaderVariable* Var;
		int Length;
		std::shared_ptr<UniformValueContainer> DefaultValue = nullptr;
	};

	struct BLT_API RendererUniformInfo
	{
	public:
		std::string VarName;
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
		std::string Source;
		std::vector<UserUniformInfo> UserUniforms;
		std::vector<RendererUniformInfo> RendererUniforms;
	};

	class BLT_API ShaderProgram
	{
	public:
		static constexpr inline const char* NAMELESS_UNIFORM = "__NAMELESS__";

	protected:
		mutable ShaderBuilder m_Builder;
		ShaderStage m_ShaderStage;

		std::vector<UserUniformPtr> m_UserUniforms;
		std::vector<RendererUniformPtr> m_RendererUniforms;

	protected:
		ShaderProgram(ShaderStage type);

	public:
		ShaderStage Type() const;

		const GlobalScope& GetGlobalScope() const;
		GlobalScope& GetGlobalScope();
		const MainScope& GetMainScope() const;
		MainScope& GetMainScope();

		ShaderVariablePtr Uniform(const std::string& linkName, ValueType type, std::shared_ptr<UniformValueContainer> defaultValue = nullptr);
		ShaderVariablePtr UniformArray(const std::string& linkName, ValueType type, uint32_t length);
		ShaderVariablePtr RendererUniform(Bolt::RendererUniform uniform);
		ShaderVariablePtr DeclareVar(ValueType type);
		ShaderVariablePtr DefineVar(const ShaderValuePtr& value);
		ShaderPassVariablePtr DeclarePassOut(ValueType type, PassType passType = PassType::None);
		ShaderPassVariablePtr DeclarePassIn(const ShaderPassVariablePtr& passOut);
		ShaderVariablePtr DeclareArray(ValueType type, uint32_t length);

		template<typename T>
		ShaderVariablePtr DeclareVar()
		{
			return DeclareVar(GetValueType<T>());
		}

		template<typename T>
		ShaderVariablePtr Uniform(const std::string& linkName)
		{
			return Uniform(linkName, GetValueType<T>());
		}

		template<typename T>
		ShaderVariablePtr Uniform(const std::string& linkName, const T& defaultValue)
		{
			return Uniform(linkName, GetValueType<T>(), std::make_shared<UniformValue<T>>(defaultValue));
		}

		template<typename T>
		ShaderVariablePtr UniformArray(const std::string& linkName, uint32_t length)
		{
			return UniformArray(linkName, GetValueType<T>(), length);
		}

		template<typename T>
		ShaderPassVariablePtr DeclarePassOut(PassType passType = PassType::None)
		{
			return DeclarePassOut(GetValueType<T>(), passType);
		}

		template<typename T>
		ShaderVariablePtr DeclareArray(uint32_t length)
		{
			return DeclareArray(GetValueType<T>(), length);
		}

		void SetVariable(const ShaderLValuePtr& var, const ShaderValuePtr& value);
		void AddAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value);
		void SubAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value);
		void MulAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value);
		void DivAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value);
		ForLoopScope& For(const ShaderVariablePtr& counter, const ShaderValuePtr& initial, const ShaderValuePtr& condition, std::unique_ptr<ShaderOp>&& iteration);
		IfScope& If(const ShaderValuePtr& condition);
		FunctionScope& DefineFunction(const std::string& name, const ValueTypeInfo& returnType, const std::vector<ValueTypeInfo>& inputs);
		bool HasFunction(const std::string& name) const;
		FunctionScope& GetFunction(const std::string& name) const;

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

		template<typename Ret, typename... Args>
		FunctionScope& DefineFunction(const std::string& name)
		{
			std::vector<ValueTypeInfo> v;
			v.reserve(sizeof...(Args));
			(v.push_back({ GetValueType<Args>(), ValueTypeDim::Single }), ...);
			return DefineFunction(name, ValueTypeInfo{ GetValueType<Ret>() }, v);
		}

	protected:
		void CompileUniformVariables(CompiledShaderProgram& program) const;		
		virtual ShaderVariablePtr PrivateStream(int streamIndex);

	};

}