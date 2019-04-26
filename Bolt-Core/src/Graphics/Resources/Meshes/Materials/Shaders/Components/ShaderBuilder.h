#pragma once
#include "Values/ShaderVariable.h"
#include "GlobalScope.h"

namespace Bolt
{

	class BLT_API ShaderBuilder
	{
	private:
		ShaderType m_ShaderType;
		blt::string m_Source;
		GlobalScope m_GlobalScope;

		mutable int m_VarCount;
		int m_CurrentScopeIndex;

		int m_CurrentCursor;

	public:
		ShaderBuilder(ShaderType shaderType);

		ShaderType GetShaderType() const;
		const blt::string& GetSource() const;
		const GlobalScope& GetGlobalScope() const;
		GlobalScope& GetGlobalScope();

		int SaveLineCursor() const;
		void LoadLineCursor(int cursor);
		void SetScopeIndex(int index);

		blt::string GetVariableName() const;

		void Write(const blt::string& str);
		void Indent();

	private:
		blt::string GetShaderTypeString() const;

	};

}