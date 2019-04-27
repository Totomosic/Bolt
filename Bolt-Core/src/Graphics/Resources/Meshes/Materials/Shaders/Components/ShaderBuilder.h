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
		mutable int m_PassCount;
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
		void NextLine();

		blt::string GetVariableName() const;
		blt::string GetPassName() const;

		void Write(const blt::string& str);
		void Indent();

		blt::string Build();

	private:
		blt::string GetShaderTypeString() const;
		void Reset();

	};

}