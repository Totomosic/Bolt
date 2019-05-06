#pragma once
#include "Values/ShaderVariable.h"
#include "Values/ShaderLiteral.h"
#include "Operations/ShaderOp.h"

namespace Bolt
{

#define BLT_DEFAULT_SHADERSCOPE_CLONE(ScopeType) inline virtual std::unique_ptr<ShaderScope> Clone() const override { return std::make_unique<ScopeType>(*this); }

	class BLT_API ShaderScope
	{
	protected:
		int m_ScopeIndex;
		std::vector<const ShaderVariable*> m_DeclaredVariables;
		std::vector<std::unique_ptr<ShaderOp>> m_Operations;

	public:
		ShaderScope();
		ShaderScope(int index);
		ShaderScope(const ShaderScope& other);
		ShaderScope& operator=(const ShaderScope& other);
		ShaderScope(ShaderScope&& other) = default;
		ShaderScope& operator=(ShaderScope&& other) = default;
		~ShaderScope() = default;

		int GetScopeIndex() const;
		bool IsDefinedInThisScope(const ShaderVariable* var) const;

		ShaderVariablePtr DefineVariable(const ShaderValuePtr& value, const blt::string& meta = "");
		ShaderVariablePtr DeclareVariable(ValueType type, const blt::string& meta = "");
		ShaderVariablePtr DeclarePassIn(const ShaderVariablePtr& outVar, const blt::string& meta = "");
		ShaderVariablePtr DeclarePassOut(ValueType type, const blt::string& meta = "");
		ShaderVariablePtr DeclareArray(ValueType type, const ShaderLiteralPtr& length, const blt::string& meta = "");
		void AddOperation(std::unique_ptr<ShaderOp>&& op);
		void InsertOperation(int index, std::unique_ptr<ShaderOp>&& op);
		ShaderScope& AddScope(std::unique_ptr<ShaderScope>&& scope);

		virtual void Build(ShaderBuilder& builder) const = 0;
		virtual std::unique_ptr<ShaderScope> Clone() const = 0;

		template<typename T, typename... Args>
		T& AddScope(Args&& ... args)
		{
			return (T&)AddScope(std::make_unique<T>(std::forward<Args>(args)...));
		}

		friend class ShaderProgram;

	protected:
		void BuildOperations(ShaderBuilder& builder) const;
		void AddDeclaredVar(const ShaderVariable* var);
	};

}