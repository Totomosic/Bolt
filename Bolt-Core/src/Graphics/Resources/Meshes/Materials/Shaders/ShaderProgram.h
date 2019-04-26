#pragma once
#include "Components/__Components__.h"

namespace Bolt
{

	struct BLT_API UserUniformMapping
	{
	public:
		blt::string LinkName;
		ShaderVariablePtr Uniform;
	};

	struct BLT_API UserUniformInfo
	{
	public:
		blt::string LinkName;
		blt::string VarName;
		ValueType Type;
	};

	struct BLT_API RendererUniformInfo
	{
	public:
		blt::string VarName;
		RendererUniform Uniform;
	};

	struct BLT_API CompiledShaderProgram
	{
	public:
		blt::string Source;
		std::vector<UserUniformInfo> UserUniforms;
		std::vector<RendererUniformInfo> RendererUniforms;
	};

	class BLT_API ShaderProgram
	{
	protected:
		ShaderBuilder m_Builder;
		ShaderType m_ShaderType;

	protected:
		ShaderProgram(ShaderType type);

	public:
		ShaderType Type() const;

		virtual CompiledShaderProgram Compile() const = 0;
		virtual void Reset();

	protected:
		void CompileUniformVariables(CompiledShaderProgram& program) const;

	};

}