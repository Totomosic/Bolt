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
		ShaderType m_Type;
		std::vector<ShaderPassValuePtr> m_PassValues;

	protected:
		ShaderProgram(ShaderType type);

	public:
		ShaderType Type() const;

		ShaderPassValuePtr Pass(ShaderValuePtr value);

		virtual CompiledShaderProgram Compile() const = 0;
		virtual void Reset();

	};

}