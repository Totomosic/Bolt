#pragma once
#include "FunctionDecl.h"
#include "BuiltInFunc.h"

namespace Bolt
{

	class BLT_API FunctionDef
	{
	public:
		struct BLT_API FunctionValueInfo
		{
		public:
			blt::string FuncString;
			ShaderValuePtr Value;
		};

	private:
		FunctionDecl m_Declaration;

		std::vector<ShaderValuePtr> m_Params;
		std::vector<FunctionValueInfo> m_FunctionResults;
		ShaderValuePtr m_OutputValue;

	public:
		FunctionDef();
		FunctionDef(const FunctionDecl& declaration);

		const FunctionDecl& Declaration() const;

		ShaderValuePtr GetParam(int index);
		void Output(ShaderValuePtr value);
		ShaderValuePtr FuncResult(const FunctionDecl& funcDecl, const std::vector<ShaderValuePtr>& inputValues);
		ShaderValuePtr FuncResult(const BuiltInFunc& funcDecl, const std::vector<ShaderValuePtr>& inputValues);

		blt::string GetSource() const;

		static blt::string GetTypeString(ValueType type);
		static blt::string GetAssignmentString(const FunctionDecl& declaration, const std::vector<ShaderValuePtr>& inputsValues);
		static blt::string GetAssignmentString(const BuiltInFunc& declaration, const std::vector<ShaderValuePtr>& inputsValues);

	private:
		blt::string GetParamName(int index) const;
		blt::string GetNextFuncResultName() const;	

	};

}