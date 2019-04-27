#pragma once
#include "Values/ShaderVariable.h"
#include "Operations/__Operations__.h"

namespace Bolt
{

	class BLT_API ShaderScope
	{
	protected:
		std::vector<std::unique_ptr<ShaderScope>> m_ChildScopes;
		ShaderScope* m_ParentScope;
		int m_ScopeIndex;

		std::vector<const ShaderVariable*> m_DeclaredVariables;
		std::vector<std::unique_ptr<ShaderOp>> m_Operations;

	public:
		ShaderScope();
		ShaderScope(int index, ShaderScope* parentScope);

		ShaderScope* ParentScope() const;
		int GetScopeIndex() const;
		bool IsDefinedInThisScope(const ShaderVariable* var) const;
		bool IsDefined(const ShaderVariable* var) const;

		ShaderVariablePtr DefineVariable(const ShaderValuePtr& value, const blt::string& meta = "");
		ShaderVariablePtr DeclareVariable(ValueType type, const blt::string& meta = "");
		ShaderVariablePtr DeclarePassIn(const ShaderVariablePtr& outVar, const blt::string& meta = "");
		ShaderVariablePtr DeclarePassOut(ValueType type, const blt::string& meta = "");
		void AddOperation(std::unique_ptr<ShaderOp>&& op);
		virtual void Build(ShaderBuilder& builder) const = 0;
		void AddChildScope(std::unique_ptr<ShaderScope>&& scope);

		ShaderScope& AddMainScope();

		friend class ShaderProgram;

	protected:
		void BuildOperations(ShaderBuilder& builder) const;
		void AddDeclaredVar(const ShaderVariable* var);
	};

}