#pragma once
#include "ShaderProgram.h"

namespace Bolt
{

	class BLT_API FragmentShader : public ShaderProgram
	{
	private:
		ShaderVariablePtr m_FragColor;

	public:
		FragmentShader();

		void SetFragColor(const ShaderValuePtr& value);
		CompiledShaderProgram Compile() const override;
		void Reset() override;

	};

}