#pragma once
#include "Components/__Components__.h"

namespace Bolt
{

	struct BLT_API CompiledShaderProgram
	{
	public:
		blt::string Source;
	};

	class BLT_API ShaderProgram
	{
	protected:
		ShaderType m_ShaderType;
		std::vector<ShaderPassValuePtr> m_PassValues;
		std::vector<ShaderRendererUniformPtr> m_RendererUniforms;
		std::vector<ShaderUniformPtr> m_Uniforms;

	protected:
		ShaderProgram(ShaderType type);

	public:
		ShaderType Type() const;

		ShaderPassValuePtr Pass(ShaderValuePtr value);
		ShaderRendererUniformPtr RendererUniform(Bolt::RendererUniform uniform);
		ShaderUniformPtr Uniform(ValueType type);
		ShaderUniformPtr Uniform(ShaderLiteralPtr defaultValue);

		template<typename T>
		ShaderUniformPtr Uniform()
		{
			return Uniform(GetValueType<T>());
		}

		virtual CompiledShaderProgram Compile() const = 0;
		virtual void Reset();

	};

}