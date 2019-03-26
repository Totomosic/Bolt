#pragma once
#include "Components/__Components__.h"

namespace Bolt
{

	struct BLT_API UserUniformMapping
	{
	public:
		blt::string LinkName;
		ShaderUniformPtr Uniform;
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
		ShaderType m_ShaderType;
		std::vector<ShaderPassValuePtr> m_PassValues;
		std::vector<ShaderRendererUniformPtr> m_RendererUniforms;
		std::vector<UserUniformMapping> m_Uniforms;

	protected:
		ShaderProgram(ShaderType type);

	public:
		ShaderType Type() const;

		virtual ShaderPassValuePtr Pass(ShaderValuePtr value);
		ShaderRendererUniformPtr RendererUniform(Bolt::RendererUniform uniform);
		ShaderUniformPtr Uniform(const blt::string& linkName, ValueType type);
		ShaderUniformPtr Uniform(const blt::string& linkName, ShaderLiteralPtr defaultValue);

		template<typename T>
		ShaderUniformPtr Uniform(const blt::string& linkName)
		{
			return Uniform(linkName, GetValueType<T>());
		}

		virtual CompiledShaderProgram Compile() const = 0;
		virtual void Reset();

	protected:
		void CompileUniformVariables(CompiledShaderProgram& program) const;

	};

}