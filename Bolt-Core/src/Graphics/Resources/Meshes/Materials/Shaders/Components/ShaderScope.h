#pragma once
#include "Values/ShaderVariable.h"
#include "Operations/__Operations__.h"

namespace Bolt
{

	class BLT_API ShaderScope
	{
	private:
		std::vector<ShaderScope> m_ChildScopes;
		ShaderScope* m_ParentScope;
		int m_ScopeIndex;

		std::vector<const ShaderVariable*> m_DefinedVariables;
		std::vector<std::unique_ptr<ShaderOp>> m_Operations;

	public:
		ShaderScope();
		ShaderScope(int index, ShaderScope* parentScope);

		ShaderScope* ParentScope() const;
		int GetScopeIndex() const;
		bool IsDefinedInThisScope(const ShaderVariable* var) const;
		bool IsDefined(const ShaderVariable* var) const;

		void DefineVariable(const ShaderVariablePtr& var);
		void AddOperation(std::unique_ptr<ShaderOp>&& op);

	private:
		void DefineVarPrivate(const ShaderVariablePtr& var);

	};

}