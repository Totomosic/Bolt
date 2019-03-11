#pragma once
#include "Components/__Components__.h"

namespace Bolt
{

	struct BLT_API CompiledShaderProgram
	{
	public:
		blt::string Source;
		std::unordered_map<ShaderPassValuePtr, blt::string> NamedPassValues;
		std::unordered_map<RendererUniform, blt::string> NamedRendererUniforms;
		std::unordered_map<ShaderUniformPtr, blt::string> NamedUserUniforms;
	};

	class BLT_API ShaderProgram
	{
	protected:
		ShaderType m_Type;
		std::vector<ShaderRendererUniformPtr> m_RendererUniforms;
		std::vector<ShaderUniformPtr> m_UserUniforms;
		std::vector<ShaderPassValuePtr> m_PassValues;

	protected:
		ShaderProgram(ShaderType type);

	public:
		ShaderType Type() const;

		ShaderRendererUniformPtr RendererUniform(Bolt::RendererUniform uniform);
		ShaderUniformPtr Uniform(ValueType type);
		ShaderUniformPtr Uniform(ShaderValuePtr defaultValue);
		ShaderPassValuePtr Pass(ShaderValuePtr value);

		virtual CompiledShaderProgram Compile() const = 0;
		virtual void Reset();

	};

}