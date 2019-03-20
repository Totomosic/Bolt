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

	protected:
		ShaderProgram(ShaderType type);

	public:
		ShaderType Type() const;

		virtual CompiledShaderProgram Compile() const = 0;
		virtual void Reset();

	};

}