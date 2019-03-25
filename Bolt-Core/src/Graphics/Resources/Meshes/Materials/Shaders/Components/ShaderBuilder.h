#pragma once
#include "Values/ShaderVariable.h"

namespace Bolt
{

	class BLT_API ShaderBuilder
	{
	private:
		ShaderType m_ShaderType;
		blt::string m_Source;
		std::vector<const ShaderVariable*> m_DeclaredVariables;

		mutable int m_UniformCount;
		mutable int m_PassCount;
		mutable int m_VarCount;

		int m_StreamCursor;
		int m_UniformCursor;
		int m_PassCursor;
		int m_CurrentCursor;

	public:
		ShaderBuilder(ShaderType shaderType);

		ShaderType GetShaderType() const;
		const blt::string& GetSource() const;

		int SaveLineCursor() const;
		void LoadLineCursor(int cursor);

		blt::string GetStreamName(int stream) const;
		blt::string GetUniformName() const;
		blt::string GetPassName() const;
		blt::string GetVariableName() const;

		bool IsDeclared(const ShaderVariable* variable) const;
		void WriteStreamLine(const blt::string& stream);
		void WriteUniformLine(const blt::string& uniform);
		void WritePassLine(const blt::string& pass);
		void Write(const blt::string& str);
		void PreviousLine();
		void NextLine();
		void DeclareVariable(const ShaderVariable* variable);

	private:
		bool CanAccessVariable(const ShaderVariable* variable) const;

	};

}