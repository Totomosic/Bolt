#pragma once
#include "Values/ShaderVariable.h"
#include "GlobalScope.h"
#include "MainScope.h"

namespace Bolt
{

	class BLT_API ShaderBuilder
	{
	private:
		ShaderStage m_ShaderStage;
		blt::string m_Source;

		GlobalScope m_GlobalScope;
		MainScope m_MainScope;

		mutable int m_VarCount;
		mutable int m_PassCount;
		int m_CurrentScopeIndex;

		int m_CurrentCursor;

	public:
		ShaderBuilder(ShaderStage shaderType);

		ShaderStage GetShaderStage() const;
		const blt::string& GetSource() const;

		const GlobalScope& GetGlobalScope() const;
		GlobalScope& GetGlobalScope();
		const MainScope& GetMainScope() const;
		MainScope& GetMainScope();

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
		blt::string GetShaderStageString() const;
		void Reset();

	};

}