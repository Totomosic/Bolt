#pragma once
#include "FunctionDecl.h"
#include "FunctionDef.h"

namespace Bolt
{
	
	class BLT_API FunctionContext
	{
	public:
		struct BLT_API FunctionImpl
		{
		public:
			FunctionDecl Declaration;
			FunctionDef Definition;
			bool IsDefined;
		};

	private:
		std::vector<FunctionImpl> m_Functions;
		std::unordered_map<blt::string, size_t> m_FunctionIndices;
		ShaderType m_Shader;

	public:
		FunctionContext(ShaderType shader);

		ShaderType Shader() const;
		int FunctionCount() const;
		bool HasFunction(const blt::string& name) const;
		bool IsDefined(const blt::string& name) const;
		const FunctionDef& GetFunction(const blt::string& name) const;
		const FunctionDecl& GetFunctionDecl(const blt::string& name) const;

		const FunctionDecl& AddFunction(const blt::string& name, const std::vector<ValueType>& inputs, ValueType outputs);
		const FunctionDecl& AddFunction(const FunctionDecl& declaration);
		void AddFunctionDefinition(const FunctionDef& definition);

		blt::string GetSource() const;

	private:
		const FunctionImpl& GetFunctionImpl(const blt::string& name) const;
		FunctionImpl& GetFunctionImpl(const blt::string& name);

	};

}